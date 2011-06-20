#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/semaphore.h>
#include <linux/kthread.h>
#include <asm/atomic.h>

static DEFINE_MUTEX( lock );
static DEFINE_SEMAPHORE( sem_lock );

static volatile int mutex_counter = 0;
static volatile int sema_counter = 0;
static atomic_t     atomic_counter;

#define NUM_THREADS     100
#define NUM_MUTEX_ITER  10000
#define NUM_SEMA_ITER   10000
#define NUM_ATOMIC_ITER 100000

static DEFINE_SEMAPHORE( thread_wait );

static inline void stuff_after_lock( void )
{
}

static inline void stuff_before_unlock( void )
{
}

static int test_mutex_thread( void *param )
{
    int     i;

    for ( i = 0; i < NUM_MUTEX_ITER; i++ )
    {
        mutex_lock( &lock );
        stuff_after_lock();
        mutex_counter++;
        stuff_before_unlock();
        mutex_unlock( &lock );
    }
    up( &thread_wait );

    return 0;
}

static int test_semaphore_thread( void *param )
{
    int     i;

    for ( i = 0; i < NUM_SEMA_ITER; i++ )
    {
        down( &sem_lock );
        stuff_after_lock();
        sema_counter++;
        stuff_before_unlock();
        up ( &sem_lock );
    }
    up( &thread_wait );

    return 0;
}

static int test_atomic_thread( void *param )
{
    int     i;

    for ( i = 0; i < NUM_ATOMIC_ITER; i++ )
    {
        atomic_inc( &atomic_counter );
    }
    up( &thread_wait );

    return 0;
}

static void launch_test_threads( const char *label, int (*test_fn)(void *param) )
{
    int i;

    printk( KERN_INFO "    Launching threads ");
    for ( i = 0; i < NUM_THREADS; i++ )
    {
        if (( i % 10 ) == 0 )
            printk( "." );
        kthread_run( test_fn, NULL, "test-%s-%d", label, i );
    }
    printk( "\n" );

    printk( KERN_INFO "    Waiting for threads to finish ");
    for ( i = 0; i < NUM_THREADS; i++ )
    {
        if (( i % 10 ) == 0 )
            printk( "." );
        down( &thread_wait );
    }
    printk( "\n" );
}

static int __init test_mutex_init( void )
{
    int counter;

    printk( KERN_INFO "Testing mutex...\n" );
    mutex_counter = 0;
    launch_test_threads( "mtx", test_mutex_thread );
    counter = mutex_counter;
    printk( KERN_INFO "  counter = %d %s\n", counter, counter == (NUM_THREADS * NUM_MUTEX_ITER) ? "Pass" : "FAIL" );

    printk( KERN_INFO "Testing semaphore...\n" );
    sema_counter = 0;
    launch_test_threads( "sem", test_semaphore_thread );
    counter = sema_counter;
    printk( KERN_INFO "  counter = %d %s\n", counter, counter == (NUM_THREADS * NUM_SEMA_ITER) ? "Pass" : "FAIL" );

    printk( KERN_INFO "Testing atomic...\n" );
    atomic_set( &atomic_counter, 0 );
    launch_test_threads( "atm", test_atomic_thread );
    counter = atomic_read( &atomic_counter );
    printk( KERN_INFO " counter = %d %s\n", counter, counter == (NUM_THREADS * NUM_ATOMIC_ITER) ? "Pass" : "FAIL" );

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

