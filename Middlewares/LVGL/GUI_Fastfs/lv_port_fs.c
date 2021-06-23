/**
 ****************************************************************************************************
 * @file        lv_port_fs_templ.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       LVGL 文件系统接口定义
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 Mini Pro H750开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#if 1

#include "lv_port_fs.h"
#include "ff.h"
#include "./BSP/SDMMC/sdmmc_sdcard.h"
#include "./FATFS/exfuns/exfuns.h"

/* 创建一个类型来存储有关文件的所需数据 */
typedef FIL file_t;

/* ' file_t '类似，也创建一个用于读取目录的类型 */
typedef DIR dir_t;

static void fs_init(void);

static lv_fs_res_t fs_open (lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br);
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw);
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos);
static lv_fs_res_t fs_size (lv_fs_drv_t * drv, void * file_p, uint32_t * size_p);
static lv_fs_res_t fs_tell (lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p);
static lv_fs_res_t fs_remove (lv_fs_drv_t * drv, const char *path);
static lv_fs_res_t fs_trunc (lv_fs_drv_t * drv, void * file_p);
static lv_fs_res_t fs_rename (lv_fs_drv_t * drv, const char * oldname, const char * newname);
static lv_fs_res_t fs_free (lv_fs_drv_t * drv, uint32_t * total_p, uint32_t * free_p);
static lv_fs_res_t fs_dir_open (lv_fs_drv_t * drv, void * rddir_p, const char *path);
static lv_fs_res_t fs_dir_read (lv_fs_drv_t * drv, void * rddir_p, char *fn);
static lv_fs_res_t fs_dir_close (lv_fs_drv_t * drv, void * rddir_p);

#define lv_debug(...) printf(__VA_ARGS__)


void lv_port_fs_init(void)
{
    /* 初始化文件系统 */
    fs_init();

    /* 在littlegl中注册文件系统接口 */
    /* 添加一个简单的驱动器 */
    lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    /* 设置字段 */
    fs_drv.file_size = sizeof(file_t);
    fs_drv.letter = '0';
    fs_drv.open_cb = fs_open;
    fs_drv.close_cb = fs_close;
    fs_drv.read_cb = fs_read;
    fs_drv.write_cb = fs_write;
    fs_drv.seek_cb = fs_seek;
    fs_drv.tell_cb = fs_tell;
    fs_drv.free_space_cb = fs_free;
    fs_drv.size_cb = fs_size;
    fs_drv.remove_cb = fs_remove;
    fs_drv.rename_cb = fs_rename;
    fs_drv.trunc_cb = fs_trunc;

    fs_drv.rddir_size = sizeof(dir_t);
    fs_drv.dir_close_cb = fs_dir_close;
    fs_drv.dir_open_cb = fs_dir_open;
    fs_drv.dir_read_cb = fs_dir_read;

    lv_fs_drv_register(&fs_drv);
}

/**********************
 *   静态函数
 **********************/

/* 初始化存储设备和文件系统 */
static void fs_init(void)
{
    exfuns_init();                 /* 为fatfs相关变量申请内存 */
    f_mount(fs[0], "0:", 1);       /* 挂载SD卡 */
//    f_mount(fs[1], "1:", 1);       /* 挂载FLASH. */
}

/**
 * 打开一个文件
 * @param drv：指向该函数所属的驱动程序
 * @param ile_p：指向file_t变量的指针
 * @param path：以驱动符开头的文件路径(例如S:/folder/file.txt)
 * @param mode: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_open (lv_fs_drv_t * drv, void * file_p, const char * path, lv_fs_mode_t mode)
{
    uint8_t flags = 0;

    if (mode == LV_FS_MODE_WR)
    {
        flags = FA_WRITE | FA_OPEN_ALWAYS;
    }
    else if (mode == LV_FS_MODE_RD)
    {
        flags = FA_READ;
    }
    else if (mode == (LV_FS_MODE_WR | LV_FS_MODE_RD))
    {
        flags = FA_READ | FA_WRITE | FA_OPEN_ALWAYS;
    }

    FRESULT res = f_open(file_p, path, flags);

    if (res == FR_OK)
    {
        f_lseek(file_p, 0);
        return LV_FS_RES_OK;
    }
    else
    {
        return LV_FS_RES_UNKNOWN;
    }
}


/**
 * 关闭已打开的文件
 * @param  drv：指向该函数所属的驱动程序
 * @param  file_p：指向file_t变量的指针。(与lv_ufs_open打开)
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p)
{
    f_close(file_p);
    return LV_FS_RES_OK;
}

/**
 * 从打开的文件中读取数据
 * @param drv：指向该函数所属的驱动程序
 * @param file_p：指向file_t变量的指针。
 * @param buf：指针，指向存储读数据的内存块
 * @param btr：要读取的字节数
 * @param br：实际读字节数(字节读)
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_read (lv_fs_drv_t * drv, void * file_p, void * buf, uint32_t btr, uint32_t * br)
{
    FRESULT res = f_read(file_p, buf, btr, (UINT *)br);

    if (res == FR_OK)
    {
        return LV_FS_RES_OK;
    }
    else
    {
        return LV_FS_RES_UNKNOWN;
    }
}

/**
 * 写入文件
 * @param  drv：   指向该函数所属的驱动程序
 * @param  file_p：指向file_t变量的指针
 * @param  buf：   指针，指向一个带有要写入字节的缓冲区
 * @param  btw：   要写的btr字节数
 * @param  br：    实际写入的字节数(已写入的字节数)。如果未使用NULL。
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_write(lv_fs_drv_t * drv, void * file_p, const void * buf, uint32_t btw, uint32_t * bw)
{
    FRESULT res = f_write(file_p, buf, btw, (UINT *)bw);
    if (res == FR_OK)
        return LV_FS_RES_OK;
    else
        return LV_FS_RES_UNKNOWN;
}

/**
 * 设置读写指针。如果需要，还可以扩展文件大小。
 * @param  drv：   指向该函数所属的驱动程序
 * @param  file_p：指向file_t变量的指针。(使用lv_ufs_open打开)
 * @param  pos：   读写指针的新位置
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos)
{
    f_lseek(file_p, pos);
    return LV_FS_RES_OK;
}

/**
 * 返回一个文件字节的大小
 * @param  drv：   指向该函数所属的驱动程序
 * @param  file_p：指向file_t变量的指针
 * @param  size：  指向一个存储大小的变量
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_size (lv_fs_drv_t * drv, void * file_p, uint32_t * size_p)
{
    (*size_p) = f_size(((file_t *)file_p));
    return LV_FS_RES_OK;
}

/**
 * 返回读写指针的位置
 * @param  drv：   指向该函数所属的驱动程序
 * @param  file_p：指向file_t变量的指针
 * @param  pos_p： 用于存储结果的指针
 * @return LLV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_tell (lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
    *pos_p = f_tell(((file_t *)file_p));
    return LV_FS_RES_OK;
}

/**
 * 删除文件
 * @param  drv： 指向该函数所属的驱动程序
 * @param  path：要删除的文件的路径
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_remove (lv_fs_drv_t * drv, const char *path)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * 截取文件
 * @param  drv：   指向该函数所属的驱动程序
 * @param  file_p：指向'ufs_file_t'变量的指针。(用lv_fs_open打开)
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_trunc (lv_fs_drv_t * drv, void * file_p)
{
    f_sync(file_p); /*If not syncronized fclose can write the truncated part*/
    f_truncate(file_p);
    return LV_FS_RES_OK;
}

/**
 * 重命名一个文件
 * @param  drv    ：指向该函数所属的驱动程序
 * @param  oldname：旧名称
 * @param  newname：新名称
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_rename (lv_fs_drv_t * drv, const char * oldname, const char * newname)
{
    FRESULT res = f_rename(oldname, newname);

    if (res == FR_OK)
        return LV_FS_RES_OK;
    else
        return LV_FS_RES_UNKNOWN;
}

/**
 * 获取驱动程序的空闲大小和总大小(kB)
 * @param  drv：    指向该函数所属的驱动程序
 * @param  total_p：存储总大小的指针[kB]
 * @param  free_p： 指针，用于存储空闲大小[kB]
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_free (lv_fs_drv_t * drv, uint32_t * total_p, uint32_t * free_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * 打开目录
 * @param  drv：    指向该函数所属的驱动程序
 * @param  rddir_p：指向'lv_fs_dir_t'变量的指针
 * @param  path：   目录路径
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_dir_open (lv_fs_drv_t * drv, void * rddir_p, const char *path)
{
    FRESULT res = f_opendir(rddir_p, path);

    if (res == FR_OK)
    {
        return LV_FS_RES_OK;
    }
    else
    {
        return LV_FS_RES_UNKNOWN;
    }
}

/**
 * 从目录中读取下一个文件名，目录名以“/”开头
 * @param  drv：    指向该函数所属的驱动程序
 * @param  rddir_p：指向初始化的“lv_fs_dir_t”变量的指针
 * @param  fn：     指向存放文件名的缓冲区的指针
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_dir_read (lv_fs_drv_t * drv, void * rddir_p, char *fn)
{
    FRESULT res;
    FILINFO fno;
    fn[0] = '\0';

    do
    {
        res = f_readdir(rddir_p, &fno);
        if (res != FR_OK)
        {
            return LV_FS_RES_UNKNOWN;
        }

        if (fno.fattrib & AM_DIR)
        {
            fn[0] = '/';
            strcpy(&fn[1], fno.fname);
        }
        else
        {
            strcpy(fn, fno.fname);
        }

    } while (strcmp(fn, "/.") == 0 || strcmp(fn, "/..") == 0);

    return LV_FS_RES_OK;
}

/**
 * 关闭目录读取
 * @param  drv：    指向该函数所属的驱动程序
 * @param  rddir_p：指向初始化的“lv_fs_dir_t”变量的指针
 * @return LV_FS_RES_OK: 成功, LV_FS_RES_NOT_IMP：失败
 */
static lv_fs_res_t fs_dir_close (lv_fs_drv_t * drv, void * rddir_p)
{
    f_closedir(rddir_p);
    return LV_FS_RES_OK;
}

#else /* Enable this file at the top */

/* This dummy typedef exists purely to silence -Wpedantic. */
typedef int keep_pedantic_happy;
#endif
