#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Carla Veronica Lopez Arteaga");
MODULE_DESCRIPTION("Lectura de estadisticas de disco para Pareja 1");

// Función que se ejecuta al cargar el módulo (insmod)
static int __init io_disco_init(void) {
    struct file *f;
    char buf[256];
    loff_t pos = 0;
    ssize_t bytes_read;

    // Variables para almacenar los sectores
    unsigned long read_sectors = 0;
    unsigned long write_sectors = 0;

    // Abrir el archivo de estadísticas de bloque
    f = filp_open("/sys/block/sda/stat", O_RDONLY, 0);
    if (IS_ERR(f)) {
        printk(KERN_ERR "Error al abrir el archivo de estadisticas del disco\n");
        return PTR_ERR(f);
    }

    // Leer el contenido usando kernel_read
    memset(buf, 0, sizeof(buf));
    bytes_read = kernel_read(f, buf, sizeof(buf) - 1, &pos);

    if (bytes_read > 0) {
        // En /sys/block/.../stat, el 3er valor es 'sectores leídos' y el 7mo es 'sectores escritos'
        // Extraemos esos valores específicos del buffer
        sscanf(buf, "%*d %*d %lu %*d %*d %*d %lu", &read_sectors, &write_sectors);

        // Reportar con printk
        printk(KERN_INFO "Estadisticas de Disco - Sectores leidos: %lu, Sectores escritos: %lu\n", 
               read_sectors, write_sectors);
    }

    // Cerrar el archivo
    filp_close(f, NULL);
    return 0;
}

// Función que se ejecuta al descargar el módulo (rmmod)
static void __exit io_disco_exit(void) {
    printk(KERN_INFO "Modulo io_disco descargado. Autor: Carla Veronica Lopez Arteaga\n");
}

// Usar los nombres nuevos en las macros
module_init(io_disco_init);
module_exit(io_disco_exit);
