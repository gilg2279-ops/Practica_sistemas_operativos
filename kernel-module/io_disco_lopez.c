#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_AUTHOR("Carla Veronica Lopez Arteaga");
MODULE_DESCRIPTION("Modulo para leer sectores leidos y escritos del disco");
MODULE_LICENSE("GPL");

static int __init init_module(void){
  struct file *filp;
  char buffer[256];
  loff_t pos = 0;
  ssize_t bytes_read;
  unsigned long read_sectors, write_sectors;
  
  flip=filp_open("/sys/block/sda/stat/", O_RDONLY, 0);
  
  if(IS_ERR(flip)){
    printk(KERN_INFO "io_disco: No se pudo abrir /sys/block/sda/stat.\n");
    return -1;
  }
  
  bytes_read = kernel_read(flip, buffer, sizeof(buffer) - 1, &pos);
  if(bytes_read>0){
    buffer[bytes_read] = '\0';
    sscanf(buffer, "%*u %*u %lu %*u %*u %*u %lu", &read_sectors, &write_sectors);
    
    printk(KERN_INFO "io_disco: Sectores leidos: %lu\n", read_sectors);
    printk(KERN_INFO "io_disco: Sectores escritos: %lu\n", write_sectors);
  }
  
  filp_close(flip, NULL);
  return 0;
  }
  
  static void __exit cleanup_module(void){
    printk(KERN_INFO "io_disco: Modulo descargado por Carla Veronica Lopez Arteaga\n");
  }
  
  module_init(init_module);
  module_exit(cleanup_module);
