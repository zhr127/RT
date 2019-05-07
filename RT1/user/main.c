 int flag1 = 0;
 int flag2 = 0;
 int flag3 = 0;

int main(void)
{
  /* */  
	HardWareInit();
	/*  */  
	for( ; ;) {
	     
		 DoSomething1();
		 
		 DoSomething2();
		 
		 DoSomething3();
	
	}
}

#ifndef   __RT_DEF_H__
 #define  __RT_DEF_H__
 
 typedef signed char          rt_int8_t;
 typedef signed short         rt_int16_t;
 typedef signed long          rt_int32_t;
 typedef unsigned char          rt_uint8_t;
 typedef unsigned short         rt_uint16_t;
 typedef unsigned long          rt_uint32_t;
 typedef int                     rt_bool_t;
 
 
 typedef long              rt_base_t;
 typedef unsigned long     rt_ubase_t;
 
 typedef rt_base_t         rt_err_t;
 typedef rt_uint32_t       rt_time_t;
 typedef rt_uint32_t       rt_tick_t;
 typedef rt_base_t         rt_flag_t;
 typedef rt_ubase_t        rt_size_t;
 typedef rt_ubase_t        rt_dev_t;
 typedef rt_base_t         rt_off_t;
 
 #define RT_TRUE      1
 #define RT_FALSE     0
 
 #ifdef __CC_ARM
 #define rt_inline        static __inline
 #define ALIGN(n)         __attribute__((aligned(n))
 
 #elif defined (__IAR_SYSTEMS_ICC__)
 #define rt_inline         static inline
 #define ALIGN(n)         PRAGMA(data_alignment=n)
 
 #elif defined  (__GNUC__)
 #define rt_inline        static __inline
 #define ALIGN(n)         __attribute__((aligned(n))
 #else
 #error not supported tool chain
 #endif
 
 #define RT_ALIGN(size, align)     (((size) + (align) - 1) & ~((align) - 1)
 
 void delay (uint32_t count)
{
	for ( ; count != 0; count--);
}


void flag1_thread_entry( void *p_arg )
{
	for( ; ; )
	{
		flag1 = 1;
		delay( 100 );
		flag1 = 0;
		delay( 100 );
	}
}

void flag2_thread_entry( void *p_arg )
{
	for( ; ; )
	{
		flag1 = 1;
		delay( 100 );
		flag1 = 0;
		delay( 100 );
	}
}

struct rt_thread
{
	void  *sp;
	void  *entry;
	void  *parameter;
	void  *stack_addr;
	rt_uint32_t  stack_size;
};
typedef rt_thread *rt_thread_t;

struct rt_thread rt_flag1_thread;
struct rt_thread rt_flag2_thread;


rt_err_t rt_thread_init(struct rt_thread *thread,
                        void (*entry) (void *parameter),
						void         *parameter,
						void         *stack_start,
						rt_uint32_t   stack_size)
						{
							rt_list_init(&(thread->tlist));
							
							thread->entry = (void *)entry;
							thread->parameter = parameter;
							
							thread->stack_addr = stack_start;
							thread->stack_size = stack_size;
							
							thread->sp =
							(void *)rt_hw_stack_init( thread->entry,
							                          thread->parameter,
							(void *)((char *)thread->stack_addr + thread->stack_size - 4));
							
							return RT_EOK;
						}

struct rt_list_node
{
	struct rt_list_node *next;
	struct rt_list_node *prev;
};
typedef struct rt_list_node rt_list_t;

rt_inline void rt_list_init(rt_list_t *l)
{
	l->next = l->prev = l;
}

rt_inline void rt_list_insert_after(rt_list_t *l, rt_list_t *n)
{
	l->next->prev = n;
	n->next = l->next;
	
	l->next = n;
	n->prev = l;
}

rt_inline void rt_list_insert_before(rt_list_t *l, rt_list_t *n)
{
	l->prev->next = n;
	n->prev = l->prev;
	
	l->prev = n;
	n->next = l;
}

rt_inline void rt_list_remove(rt_list_t *n)
{
	n->next->prev = n->prev;
	n->prev->next = n->next;
	
	n->next = n->prev = n;
}

rt_uint8_t *rt_hw_stack_init(void  *tentry,
                             void  *parameter,
							 rt_unint8_t *stack_addr)
{
	struct stack_frame *stack_frame;
	rt_uint8_t         *stk;
	unsigned long        i;
	
	stk = stack_addr + sizeof(rt_uint32_t);
	
	stk = (rt_uint8_t *)RT_ALIGN_DOWN((rt_uint32_t)stk, 8);
	
	stk -= sizeof(struct stack_frame);
	
	stack_frame = (struct stack_frame *)stk;
	
	for (i = 0; i < sizeof(struct stack_frame) / sizeof(rt_uint32_t); i++)
	{
		((rt_uint32_t *)stack_frame)[i] = 0xdeadbeef;
	}
	
	stack_frame->execption_stack_frame.r0 = (unsigned long)parameter;
	stack_frame->execption_stack_frame.r1 = 0;
	stack_frame->execption_stack_frame.r2 = 0;
	stack_frame->execption_stack_frame.r3 = 0;
	stack_frame->execption_stack_frame.r12 = 0;
	stack_frame->execption_stack_frame.lr = 0;
	stack_frame->execption_stack_frame.pc = (unsigned long)tentry;
	stack_frame->execption_stack_frame.psr = 0x01000000L;
	
	return stk;
}
