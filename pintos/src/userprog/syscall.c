#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "userprog/exception.h"
#include "userprog/process.h"
#include <devices/input.h>

static void syscall_handler(struct intr_frame *);
void halt(void);
void exit(int status);
int wait(pid_t pid);
int read(int fd, void *buffer, unsigned size);
int write(int fd, const void *buffer, unsigned size);
pid_t exec(const char *cmd_line);

void syscall_init(void)
{
  intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler(struct intr_frame *f UNUSED)
{
  //printf("wowowo\n");
  //printf("system call! gogogo\n");
  //printf("syscall num : %d\n", *(uint32_t *)(f->esp));
  //printf("the stack pointer of syscall.c is : %X\n", (f->esp + 4));
  //f->esp = 0xBFFFFFE0;
  //hex_dump(f->esp, f->esp, 100, 1);
  //printf("fd: %hu, size: %i\n", *(uint32_t *)0xBFFFFFE0 + 4);
  //printf("%d, %s, %d\n", (int)*(head + 4), (const void *)*(head + 8), (unsigned)*(head + 12));

  int *sc_num = f->esp;

  if (!is_user_vaddr(sc_num))
    if (!is_user_vaddr(sc_num))
      exit(-1);

  switch (*sc_num)
  {
  case SYS_HALT:
    halt();
    break;

  case SYS_EXIT:
    if (!is_user_vaddr(&sc_num[1]))
      exit(-1);
    exit(sc_num[1]);
    break;

  case SYS_EXEC:
    if (!is_user_vaddr(&sc_num[1]))
      exit(-1);
    f->eax = (uint32_t)exec((const char *)sc_num[1]);
    break;

  case SYS_WAIT:
    if (!is_user_vaddr(&sc_num[1]))
      exit(-1);
    f->eax = (uint32_t)wait((pid_t)sc_num[1]);
    break;

  case SYS_READ:
    if (!is_user_vaddr(&sc_num[1]) || !is_user_vaddr(&sc_num[2]) || !is_user_vaddr(&sc_num[3]))
      exit(-1);

    f->eax = (uint32_t)read((int)sc_num[1], (void *)sc_num[2], (unsigned)sc_num[3]);
    break;

  case SYS_WRITE:
    if (!is_user_vaddr(&sc_num[1]) || !is_user_vaddr(&sc_num[2]) || !is_user_vaddr(&sc_num[3]))
      exit(-1);
    f->eax = (uint32_t)write((int)sc_num[1], (const void *)sc_num[2], (unsigned)sc_num[3]);
    break;

  case SYS_FIBO:
    if (!is_user_vaddr(&sc_num[1]))
      exit(-1);
    f->eax = (uint32_t)fibonacci((int)sc_num[1]);
    break;
  case SYS_MAX_FOUR:
    if (!is_user_vaddr(&sc_num[1]) || !is_user_vaddr(&sc_num[2]) || !is_user_vaddr(&sc_num[3]) || !is_user_vaddr(&sc_num[4]))
      exit(-1);
    f->eax = (uint32_t)max_of_four_int((int)sc_num[1], (int)sc_num[2], (int)sc_num[3], (int)sc_num[4]);
    break;

  default:
    thread_exit();
    break;
  }
}

void halt()
{
  shutdown_power_off();
}

void exit(int status)
{
  struct thread *now = thread_current();
  now->exit_status = status;
  printf("%s: exit(%d)\n", now->name, now->exit_status);
  list_remove(&(now->child_elem));

  if (now->parent)
    now->parent->exit_status = now->exit_status;
  thread_exit();
}

int wait(pid_t pid)
{
  return process_wait((tid_t)pid);
}

int read(int fd, void *buffer, unsigned size)
{
  int ret = -1;

  if (fd == 0)
  {
    ret = 0;
    while (input_getc() != '\0')
    {
      ret++;
    }
  }
  else
  {
    ret = ret;
  }

  return ret;
}

int write(int fd, const void *buffer, unsigned size)
{
  int ret = -1;
  if (fd == 1)
  {
    //printf("end with senario write\n");
    putbuf(buffer, size);
    ret = size;
  }
  else
  {
    /*TODO NOT NOW FOR NOW*/
  }

  return ret;
}
pid_t exec(const char *cmd_line)
{
  pid_t cpid;
  cpid = process_execute(cmd_line);
  return cpid;
}

int fibonacci(int n)
{
  printf("the numbers are %d\n", n);
  int now = 1, prev = 0, result = -1;
  n = n + 1;
  if (n < 0)
  {
    result = -1;
  }
  else if (n == 0)
  {
    result = 0;
  }
  else if (n == 1 || n == 2)
  {
    result = 1;
  }
  else
  {
    for (int i = 3; i <= n; i++)
    {
      result = now + prev;
      prev = now;
      now = result;
    }
  }
  return result;
}

int max_of_four_int(int a, int b, int c, int d)
{

  printf("the numbers are %d %d %d %d\n", a, b, c, d);

  int maxi = a;
  if (maxi < b)
    maxi = b;
  if (maxi < c)
    maxi = c;
  if (maxi < d)
    maxi = d;
  return maxi;
}