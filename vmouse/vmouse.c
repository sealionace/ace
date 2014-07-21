/*
 * Driver de mouse virtual
 *
 * Autor: José Carlos
 */
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/module.h>

struct input_dev *vmouse_input_dev;
static struct platform_device *vmouse_dev;

/*
 * verifica se um char é espaço (a biblioteca ctype.h não está disponível em kernel-space)
 */
int is_space(int c)
{
	if (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '\v' || c == '\f')
		return 1;
	return 0;
}

/*
 * verifica se uma string está vazia
 */
int is_empty(const char *str)
{
	while (is_space(*str))
		str++;
	
	if (*str == 0) // null
		return 1;
	
	return 0;
}

/* método que trata a escrita em sysfs */
static ssize_t vmouse_write(
		struct device *dev,
		struct device_attribute *attr,
		const char *buffer,
		size_t count)
{
	char comando, variante, variante2;
	int p1, p2;
	int lido; /* quantos caracteres foram consumidos */

	/* lê a entrada */
	if (1 != sscanf(buffer, "%c%n", &comando, &lido))
	{
		printk(KERN_INFO "vmouse: erro de leitura\n");
		return count;
	}

	switch (comando)
	{
	case 'm': /* movimento: r deltaX deltaY | a posX posY */
		if (4 == sscanf(buffer, "%c%c%d%d%n", &comando, &variante, &p1, &p2, &lido))
		{
			if (variante == 'r') /* relativo */
			{
				input_report_rel(vmouse_input_dev, REL_X, p1);
				input_report_rel(vmouse_input_dev, REL_Y, p2);
				printk(KERN_INFO "vmouse_m: rel X:%d Y:%d\n", p1, p2);
			}
			else if (variante == 'a') /* absoluto */
			{
				input_report_key(vmouse_input_dev, BTN_TOUCH, 1);
				input_report_key(vmouse_input_dev, BTN_TOUCH, 0);
				
				input_report_abs(vmouse_input_dev, ABS_X, p1);
				input_report_abs(vmouse_input_dev, ABS_Y, p2);
				printk(KERN_INFO "vmouse_m: abs X:%d Y:%d\n", p1, p2);
			}
			else
			{
				printk(KERN_INFO "vmouse_m: %c invalido\n", variante);
				return lido;
			}
		}
		else
		{
			printk(KERN_INFO "vmouse_m: erro de leitura\n");
			return count;
		}

		break;

	case 'c': /* clique: [p|r]r|l|m */
		if (3 == sscanf(buffer, "%c%c%c%n", &comando, &variante2, &variante, &lido)
			&& !is_space(variante)
		) /* clicar e segurar */
		{
			if (variante2 == 'p') /* pressionar */
			{
				if (variante == 'r') /* botão direito */
				{
					input_report_key(vmouse_input_dev, BTN_RIGHT, 1);
					printk(KERN_INFO "vmouse_c: right-press\n");
				}
				else if (variante == 'l') /* botão esquerdo */
				{
					input_report_key(vmouse_input_dev, BTN_LEFT, 1);
					printk(KERN_INFO "vmouse_c: left-press\n");
				}
				else if (variante == 'm') /* botão do meio */
				{
					input_report_key(vmouse_input_dev, BTN_MIDDLE, 1);
					printk(KERN_INFO "vmouse_c: middle-press\n");
				}
				else
				{
					printk(KERN_INFO "vmouse_c: press %c invalido\n", variante);
					return lido;
				}
			}
			else if (variante2 == 'r') /* soltar */
			{
				if (variante == 'r') /* botão direito */
				{
					input_report_key(vmouse_input_dev, BTN_RIGHT, 0);
					printk(KERN_INFO "vmouse_c: right-release\n");
				}
				else if (variante == 'l') /* botão esquerdo */
				{
					input_report_key(vmouse_input_dev, BTN_LEFT, 0);
					printk(KERN_INFO "vmouse_c: left-release\n");
				}
				else if (variante == 'm') /* botão do meio */
				{
					input_report_key(vmouse_input_dev, BTN_MIDDLE, 0);
					printk(KERN_INFO "vmouse_c: middle-release\n");
				}
				else
				{
					printk(KERN_INFO "vmouse_c: release %c invalido\n", variante);
					return lido;
				}
			}
			else
			{
				printk(KERN_INFO "vmouse_c: %c%c invalido\n", variante2, variante);
				return lido;
			}
		}
		else if (2 == sscanf(buffer, "%c%c%n", &comando, &variante, &lido)) /* clique simples */
		{
			if (variante == 'r') /* botão direito */
			{
				input_report_key(vmouse_input_dev, BTN_RIGHT, 1);
				input_report_key(vmouse_input_dev, BTN_RIGHT, 0);
				printk(KERN_INFO "vmouse_c: right-click\n");
			}
			else if (variante == 'l') /* botão esquerdo */
			{
				input_report_key(vmouse_input_dev, BTN_LEFT, 1);
				input_report_key(vmouse_input_dev, BTN_LEFT, 0);
				printk(KERN_INFO "vmouse_c: left-click\n");
			}
			else if (variante == 'm') /* botão do meio */
			{
				input_report_key(vmouse_input_dev, BTN_MIDDLE, 1);
				input_report_key(vmouse_input_dev, BTN_MIDDLE, 0);
				printk(KERN_INFO "vmouse_c: middle-click\n");
			}
			else
			{
				printk(KERN_INFO "vmouse_c: click %c invalido\n", variante);
				return lido;
			}
		}
		else
		{
			printk(KERN_INFO "vmouse_c: erro de leitura\n");
			return count;
		}
		break;

	case 'w': /* rolagem: v deltaV | h deltaH */
		if (3 == sscanf(buffer, "%c%c%d%n", &comando, &variante, &p1, &lido))
		{
			if (variante == 'v') /* vertical */
			{
				input_report_rel(vmouse_input_dev, REL_WHEEL, p1);
				printk(KERN_INFO "vmouse_w: vert %d\n", p1);
			}
			else if (variante == 'h') /* horizontal*/
			{
				input_report_rel(vmouse_input_dev, REL_HWHEEL, p1);
				printk(KERN_INFO "vmouse_w: hor %d\n", p1);
			}
			else
			{
				printk(KERN_INFO "vmouse_w: %c invalido\n", variante);
				return lido;
			}
		}
		else
		{
			printk(KERN_INFO "vmouse_w: erro de leitura\n");
			return count;
		}
		break;
	default:
		if (is_empty(buffer))
		{
			return count;
		}
		
		printk(KERN_INFO "vmouse: comando %c invalido\n", comando);
		return lido;
		break;
	}

	input_sync(vmouse_input_dev);
	return lido;
}

/* vincula o método de escrita em sysfs */
DEVICE_ATTR(event, 0777, NULL, vmouse_write);

/* atributos para criação no sysfs */
static struct attribute *vmouse_attrs[] = {
		&dev_attr_event.attr,
		NULL
};

static struct attribute_group vmouse_attr_group = {
		.attrs = vmouse_attrs,
};

/* inicialização do driver */
int __init vmouse_init(void)
{
	/* registra o driver */
	vmouse_dev = platform_device_register_simple("vmouse", -1, NULL, 0);
	if (IS_ERR(vmouse_dev)) {
		printk(KERN_INFO "vmouse_init: erro\n");
		return PTR_ERR(vmouse_dev);
	}

	/* cria um nó sysfs para ler os comandos */
	sysfs_create_group(&vmouse_dev->dev.kobj, &vmouse_attr_group);

	/* aloca um input_device */
	vmouse_input_dev = input_allocate_device();
	if (!vmouse_input_dev) {
		printk(KERN_INFO "vmouse_init: falha ao alocar dispositivo\n");
		return -ENOMEM;
	}

	/* registra as capacidades do dispositivo */
	set_bit(EV_REL, vmouse_input_dev->evbit);
	set_bit(REL_X, vmouse_input_dev->relbit);
	set_bit(REL_Y, vmouse_input_dev->relbit);
	set_bit(REL_WHEEL, vmouse_input_dev->relbit);
	set_bit(REL_HWHEEL, vmouse_input_dev->relbit);

	set_bit(EV_KEY, vmouse_input_dev->evbit);
	set_bit(BTN_LEFT, vmouse_input_dev->keybit);
	set_bit(BTN_MIDDLE, vmouse_input_dev->keybit);
	set_bit(BTN_RIGHT, vmouse_input_dev->keybit);
	set_bit(BTN_TOUCH, vmouse_input_dev->keybit);

	/* abs requer mais coisinhas chatas */
	set_bit(EV_ABS, vmouse_input_dev->evbit);
	set_bit(ABS_X, vmouse_input_dev->absbit);
	set_bit(ABS_Y, vmouse_input_dev->absbit);

	input_set_abs_params(vmouse_input_dev, ABS_X, 0, 100, 0, 0);
	input_set_abs_params(vmouse_input_dev, ABS_Y, 0, 100, 0, 0);

	/* registra o dispositivo no subsistema de entrada */
	input_register_device(vmouse_input_dev);

	printk(KERN_INFO "vmouse: inicializado com sucesso\n");

	return 0;
}

/* desativação do driver */
void vmouse_uninit(void)
{
	/* desregistra o dispositivo no subsistema de entrada */
	input_unregister_device(vmouse_input_dev);

	/* remove o nó sysfs */
	sysfs_remove_group(&vmouse_dev->dev.kobj, &vmouse_attr_group);

	/* desregistra o driver */
	platform_device_unregister(vmouse_dev);

	return;
}

module_init(vmouse_init);
module_exit(vmouse_uninit);

MODULE_AUTHOR("José Carlos <junior19971103@hotmail.com>");
MODULE_DESCRIPTION("Driver de mouse virtual");
MODULE_LICENSE("GPL");
