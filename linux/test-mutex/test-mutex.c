#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>

#define TEST_MUTEX  1

#if TEST_MUTEX
static DEFINE_MUTEX( lock );
#else
static DEFINE_SEMAPHORE( sem_lock );
#endif

static volatile int counter = 0;

#define NUM_THREADS 100

static DEFINE_SEMAPHORE( thread_wait );

static int test_mutex_thread( void *param )
{
    int     i;

    for ( i = 0; i < 10000; i++ )
    {
#if TEST_MUTEX
        mutex_lock( &lock );
#else
        down( &sem_lock );
#endif
        mb();
        dsb();
        counter++;
        mb();
        dsb();
#if TEST_MUTEX
        mutex_unlock( &lock );
#else
        up ( &sem_lock );
#endif
    }
    up( &thread_wait );

    return 0;
}

static int __init test_mutex_init( void )
{
    int     i;

    counter = 0;

#if TEST_MUTEX
    printk( "Testing mutex...\n" );
#else
    printk( "Testing semaphore...\n" );
#endif

    printk( "Launching threads ...\n");
    for ( i = 0; i < NUM_THREADS; i++ )
    {
        printk( "." );
        kthread_run( test_mutex_thread, NULL, "test-mtx-%d", i );
    }
    printk( "\n" );

    printk( "Waiting for threads to finish...\n");
    for ( i = 0; i < NUM_THREADS; i++ )
    {
        printk( "." );
        down( &thread_wait );
    }
    printk( "\n" );

    printk( "counter = %d\n", counter );

    return 0;
}

static void __exit test_mutex_exit( void )
{
    printk( KERN_INFO "%s: called\n", __func__ );
}

module_init(test_mutex_init);
module_exit(test_mutex_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("dhylands@gmail.com");

