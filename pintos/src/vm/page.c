#include "page.h"
#include "threads/vaddr.h"
#include "threads/thread.h"
#include "threads/malloc.h"
#include "userprog/pagedir.h"
#include "filesys/file.h"
#include "threads/interrupt.h"
#include <string.h>

void vm_init(struct hash *vm)
{
    /* hash_init()으로 해시테이블 초기화 */
    /* 인자로 해시 테이블과 vm_hash_func과 vm_less_func 사용 */
    ASSERT(vm != NULL);
    hash_init(vm, vm_hash_func, vm_less_func, 0);
}

static unsigned vm_hash_func(const struct hash_elem *e, void *aux)
{
    /* hash_entry()로 element에 대한 vm_entry 구조체 검색 */
    /* hash_int()를 이용해서 vm_entry의 멤버 vaddr에 대한 해시값을 구하고 반환 */
    ASSERT(e != NULL);
    int temp = hash_entry(e, struct vm_entry, elem)->vaddr;
    return hash_int(temp);
}

static bool vm_less_func(const struct hash_elem *a, const struct hash_elem *b)
{
    /* hash_entry()로 각각의 element에 대한 vm_entry 구조체를 얻은 후 vaddr 비교 (b가 크다면 true, a가 크다면 false */
    ASSERT(a != NULL);
    ASSERT(b != NULL);

    int temp_a = hash_entry(a, struct vm_entry, elem)->vaddr;
    int temp_b = hash_entry(b, struct vm_entry, elem)->vaddr;
    return temp_a < temp_b;
}

bool insert_vme(struct hash *vm, struct vm_entry *vme)
{
    ASSERT(vm != NULL);
    ASSERT(vme != NULL);

    if (hash_insert(vm, vme) == NULL)
        return true;
    return false;
}

bool delete_vme(struct hash *vm, struct vm_entry *vme)
{
    ASSERT(vm != NULL);
    ASSERT(vme != NULL);

    if (hash_delete(vm, vme) != NULL)
        return true;
    return false;
}

struct vm_entry *find_vme(void *vaddr) //일단 수정 보류
{
    /* pg_round_down()으로 vaddr의 페이지 번호를 얻음 */
    /* hash_find() 함수를 사용해서 hash_elem 구조체 얻음 */
    /* 만약 존재하지 않는다면 NULL 리턴 */
    /* hash_entry()로 해당 hash_elem의 vm_entry 구조체 리턴 */

    struct hash *vm = &thread_current()->vm;
    struct vm_entry vme;
    struct hash_elem *elem;

    int vadder_page = pg_round_down(vaddr);
    vme.vaddr = pg_round_down(vaddr);
    ASSERT(pg_ofs(vme.vaddr) == 0);
    elem = hash_find(vm, &vme.elem);
    return elem ? hash_entry(elem, struct vm_entry, elem) : NULL;
}
void vm_destroy(struct hash *vm)
{
    hash_destroy(vm, vm_destroy_func);
}

/* Destroys hash table H.

   If DESTRUCTOR is non-null, then it is first called for each
   element in the hash.  DESTRUCTOR may, if appropriate,
   deallocate the memory used by the hash element.  However,
   modifying hash table H while hash_clear() is running, using
   any of the functions hash_clear(), hash_destroy(),
   hash_insert(), hash_replace(), or hash_delete(), yields
   undefined behavior, whether done in DESTRUCTOR or
   elsewhere. */
void vm_destroy_func(struct hash_elem *e, void *aux)
{
    ASSERT(e != NULL);
    struct vm_entry *vme = hash_entry(e, struct vm_entry, elem);
    //free_page_vaddr(vme->vaddr);
    //swap_clear(vme->swap_slot);
    free(vme);
}
