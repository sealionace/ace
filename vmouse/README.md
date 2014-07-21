# Driver de mouse virtual

Esta pasta contém os códigos-fonte para implantar um driver de mouse virtual no Linux.

### Compilação

Para compilar, execute o ``make``. ``make clean`` apagará os arquivos-objeto da compilação.

### Instalação

Para instalar execute ``sudo insmod vmouse.ko`` (depois de compilar, claro).

## Uso

> Se o mouse é virtual como vou mexer ele?

O driver lê comandos em `/sys/devices/platform/vmouse/event`. Por exemplo:

##### Movimento relativo

``echo "mr 10 -15" > /sys/devices/platform/vmouse/event``

**Entrada:** ``mr deltaX deltaY``

##### Movimento absoluto

``echo "ma 100 40" > /sys/devices/platform/vmouse/event``

**Entrada:** ``ma posX posY``

##### Clique

``echo "cr" > /sys/devices/platform/vmouse/event``
``echo "cpl" > /sys/devices/platform/vmouse/event``

**Entrada:** ``c[p|r](r|l|m)``
> *p* e *r* são opcionais (segundo parâmetro); indicam pressionar (*press*) e soltar (*release*). Se não especificado, o comportamento é de clique (pressionar seguido de soltar).

> *r* para clique com o botão direito, *l* para clique com o botão esquerdo e *m* para clique com o botão do meio.

##### Rolagem vertical

``echo "wv 10" > /sys/devices/platform/vmouse/event``

**Entrada:** ``wv deltaV``

##### Rolagem horizontal

``echo "wh -10" > /sys/devices/platform/vmouse/event``

**Entrada:** ``wh deltaH``

## TODO

* A implantação provê uma interface simples para comunicação com o Node.js via sistema de arquivos, porém não acho que a escrita em sysfs esteja interessante (o caminho principalmente, fica gigante_. Se houver tempo, pretendo criar uma interface D-Bus para a comunicação.
* Tentar descobrir como fazer o movimento absoluto funcionar :/
