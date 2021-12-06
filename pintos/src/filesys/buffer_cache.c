#include <string.h>
#include <debug.h>
#include <stdlib.h>
#include "threads/palloc.h"
#include "filesys/buffer_cache.h"

#define BUFFER_CACHE_ENTRY_NB 64

static struct buffer_cache_entry buffer_head[BUFFER_CACHE_ENTRY_NB]; /*metadata for p_buffer_cache*/

static char *p_buffer_cache; /*real data*/

/*clock 알고리즘 용도*/
static struct buffer_cache_entry *clock_hand;

/* buffer_head 배열에 새로운 값을 추가하거나 뺄 때 중간 과정을 보이지 않게 하는 락입니다.*/
static struct lock buffer_cache_lock;

bool bc_read(block_sector_t sector_idx, void *buffer, off_t bytes_read, int chunk_size, int sector_ofs)
{
    /* sector_idx를 buffer_head에서 검색 (bc_lookup 함수 이용) */
    /* 검색 결과가 없을 경우, 디스크 블록을 캐싱 할 buffer entry의 buffer_head를 구함 (bc_select_victim 함수 이용) */
    /* block_read 함수를 이용해, 디스크 블록 데이터를 buffer_cache 로 read */
    /* memcpy 함수를 통해, buffer에 디스크 블록 데이터를 복사 */
    /* buffer_head의 clock bit을 setting */

    struct buffer_cache_entry *cur = bc_lookup(sector_idx);
    if (cur == NULL)
    {
        cur = bc_select_victim();
        cur->valid_bit = true;
        cur->dirty_bit = false;
        cur->disk_sector = sector_idx;
        lock_release(&buffer_cache_lock); // lock
        block_read(fs_device, sector_idx, cur->buffer);
    }
    memcpy(buffer + bytes_read, cur->buffer + sector_ofs, chunk_size);

    cur->reference_bit = true;
    return true;
}

bool bc_write(block_sector_t sector_idx, void *buffer, off_t bytes_written, int chunk_size, int sector_ofs)
{

    bool success = false;
    /* sector_idx를 buffer_head에서 검색하여 buffer에 복사(구현)*/
    /* update buffer head (구현) */
    struct buffer_cache_entry *cur = bc_lookup(sector_idx);

    if (cur == NULL)
    {
        cur = bc_select_victim();
        cur->valid_bit = true;
        cur->disk_sector = sector_idx;
        lock_release(&buffer_cache_lock); //lock
        block_read(fs_device, sector_idx, cur->buffer);
    }
    memcpy(buffer + bytes_written, cur->buffer + sector_ofs, chunk_size);
    cur->reference_bit = true;

    return success;
}

void bc_init(void)
{
    /* Allocation buffer cache in Memory */
    /* p_buffer_cache가 buffer cache 영역 포인팅 */
    /* 전역변수 buffer_head 자료구조 초기화 */
    struct buffer_cache_entry *cur;
    for (int i = 0; i < BUFFER_CACHE_ENTRY_NB; i++)
    {
        memset(&buffer_head[i], 0, sizeof(struct buffer_cache_entry));
    }
    clock_hand = buffer_head;
    lock_init(&buffer_cache_lock);
}
void bc_term(void)
{
    /* bc_flush_all_entries 함수를 호출하여 모든 buffer cache entry를 디스크로 flush */
    /* buffer_cache 영역 할당 해제 */
    bc_flush_all_entries();
}

struct buffer_cache_entry *bc_select_victim(void)
{
    /* clock 알고리즘을 사용하여 victim entry를 선택 */
    /* buffer_head 전역변수를 순회하며 clock_bit 변수를 검사 */
    /* 선택된 victim entry가 dirty일 경우, 디스크로 flush */
    /* victim_entry에 해당하는 buffer_head 값 update */
    /* victim_entry를 return */
    int count = 2;
    int i = 0;
    while (count < 2)
    {
        for (; clock_hand < buffer_head + BUFFER_CACHE_ENTRY_NB; clock_hand++)
        {
            if (!(clock_hand->valid_bit) || !(clock_hand->reference_bit))
            {
                return clock_hand++;
            }
            clock_hand->reference_bit = false;
        }
        clock_hand = buffer_head;
        count++;
    }
}

struct buffer_cache_entry *bc_lookup(block_sector_t sector)
{
    /* buffe_head를 순회하며, 전달받은 sector 값과 동일한
sector 값을 갖는 buffer cache entry가 있는지 확인 */
    /* 성공 : 찾은 buffer_head 반환, 실패 : NULL */
    //lock_acquire(&buffer_cache_lock);
    for (int i = 0; i < BUFFER_CACHE_ENTRY_NB; i++)
    {
        if (buffer_head[i].valid_bit && buffer_head[i].disk_sector == sector)
        {
            //lock_release(&buffer_cache_lock);
            return &(buffer_head[i]);
        }
    }
    return NULL;
}

void bc_flush_entry(struct buffer_cache_entry *p_flush_entry)
{
    /* block_write을 호출하여, 인자로 전달받은 buffer cache entry의 데이터를 디스크로 flush */
    /* buffer_head의 dirty 값 update */
    if (!p_flush_entry->valid_bit)
        return;
    if (!p_flush_entry->dirty_bit)
        return;
    p_flush_entry->dirty_bit = false;
    block_write(fs_device, p_flush_entry->disk_sector, p_flush_entry->buffer);
}

void bc_flush_all_entries(void)
{
    /* 전역변수 buffer_head를 순회하며, dirty인 entry는 block_write 함수를 호출하여 디스크로 flush */
    /* 디스크로 flush한 후, buffer_head의 dirty 값 update */
    //buffer_head
    lock_acquire(&buffer_cache_lock);
    for (int i = 0; i < BUFFER_CACHE_ENTRY_NB; i++)
    {
        bc_flush_entry(&buffer_head[i]);
    }
    lock_release(&buffer_cache_lock);
}
