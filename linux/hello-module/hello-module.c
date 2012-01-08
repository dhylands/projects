/****************************************************************************
*
*   Kernel version of hello-world, basically, a super simple module.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <linux/module.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/****************************************************************************
*
*  hello_init
*
*     Initialization function called when the module is loaded.
*
****************************************************************************/

static int __init hello_init( void )
{
    printk( "hello_init called\n" );

    return 0;

} // hello_init

/****************************************************************************
*
*  hello_exit
*
*       Called to perform module cleanup when the module is unloaded.
*
****************************************************************************/

static void __exit hello_exit( void )
{
    printk( "hello_exit called\n" );

} // hello_exit

/****************************************************************************/

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Dave Hylands");
MODULE_DESCRIPTION("Hello World Module");
MODULE_LICENSE("GPL");

