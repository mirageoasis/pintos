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

  if (!is_user_vaddr(f->esp))
    exit(-1);

  switch (*(uint32_t *)(f->esp))
  {
  case SYS_HALT:
    /* code */
    halt();
    break;
  case SYS_EXIT:
    /* code */
    if (!is_user_vaddr((f->esp + 4)))
      exit(-1);
    exit(*(int *)(f->esp + 4));
    break;
  case SYS_EXEC:
    /* code */
    if (!is_user_vaddr((uint32_t *)(f->esp + 4)))
      exit(-1);
    //printf("exec %s\n", (const char *)(f->esp + 4));
    f->eax = exec((const char *)(f->esp + 4));
    break;
  case SYS_WAIT:
    /* code */
    if (!is_user_vaddr(f->esp + 4))
      exit(-1);
    f->eax = (uint32_t)wait((pid_t)f->esp + 4);
    break;
  case SYS_CREATE:
    /* code */
    break;
  case SYS_REMOVE:
    /* code */
    break;
  case SYS_OPEN:
    /* code */
    break;
  case SYS_FILESIZE:
    /* code */
    break;
  case SYS_READ:
    /* code */
    if (!is_user_vaddr(f->esp + 4) || !is_user_vaddr(f->esp + 8) || !is_user_vaddr(f->esp + 12))
      exit(-1);
    f->eax = read((int)*(uint32_t *)(f->esp + 4), (void *)*(uint32_t *)(f->esp + 8), (unsigned)*((uint32_t *)(f->esp + 12)));
    break;
  case SYS_WRITE:
    //write();
    if (!is_user_vaddr(f->esp + 4) || !is_user_vaddr(f->esp + 8) || !is_user_vaddr(f->esp + 12))
      exit(-1);
    f->eax = write((int)*(uint32_t *)(f->esp + 4), (void *)*(uint32_t *)(f->esp + 8), (unsigned)*((uint32_t *)(f->esp + 12)));
    break;
  case SYS_SEEK:
    /* code */
    break;
  case SYS_TELL:
    /* code */
    break;
  case SYS_CLOSE:
    /* code */
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
  //printf("im in the exit!\n");
  struct thread *now = thread_current();
  now->end_true = true;
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
  //printf("execute your area\n");
  char temp_name[128];
  int i = 0;
  for (int i = 0; cmd_line[i] != '\0' && cmd_line[i] != ' '; i++)
  {
  }
  strlcpy(temp_name, cmd_line, i);
  temp_name[i] = '\0';

  if (filesys_open(temp_name) == NULL)
  {
    return -1;
  }
  printf("the execute is %s\n", temp_name);
  tid_t ret = process_execute(temp_name);
  //process_wait(ret);
  sema_down(&(thread_current()->sema_load));

  return (pid_t)ret;
}