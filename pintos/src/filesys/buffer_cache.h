#include "filesys/filesys.h"
#include "filesys/inode.h"
#include "threads/synch.h"

struct buffer_cache_entry
{
    // 더러운지를 나타냅니다.
    bool dirty_bit;

    // 사용 중인지를 나타냅니다.
    bool valid_bit;

    // 캐시된 섹터 번호입니다.
    block_sector_t disk_sector;

    // clock 알고리즘에서 사용합니다.
    bool reference_bit;

    // 쓰기 작업을 하기 전에 이 락을 획득합니다.
    //struct lock lock;
    // 데이터 버퍼를 가리킵니다.

    uint8_t buffer[BLOCK_SECTOR_SIZE];
};

void bc_init(void);
void bc_term(void);
bool bc_read(block_sector_t, void *, off_t, int, int);
bool bc_write(block_sector_t, void *, off_t, int, int);
struct buffer_cache_entry *bc_lookup(block_sector_t);
struct buffer_cache_entry *bc_select_victim(void);
void bc_flush_entry(struct buffer_cache_entry *);
void bc_flush_all_entries(void);
