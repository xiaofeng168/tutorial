/**
 ****************************************************************************************************
 * @file        lv_port_fs_templ.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-03-29
 * @brief       LVGL �ļ�ϵͳ�ӿڶ���
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� Mini Pro H750������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#if 1

#include "lv_port_fs.h"
#include "ff.h"
#include "./BSP/SDMMC/sdmmc_sdcard.h"
#include "./FATFS/exfuns/exfuns.h"

/* ����һ���������洢�й��ļ����������� */
typedef FIL file_t;

/* ' file_t '���ƣ�Ҳ����һ�����ڶ�ȡĿ¼������ */
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
    /* ��ʼ���ļ�ϵͳ */
    fs_init();

    /* ��littlegl��ע���ļ�ϵͳ�ӿ� */
    /* ���һ���򵥵������� */
    lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);

    /* �����ֶ� */
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
 *   ��̬����
 **********************/

/* ��ʼ���洢�豸���ļ�ϵͳ */
static void fs_init(void)
{
    exfuns_init();                 /* Ϊfatfs��ر��������ڴ� */
    f_mount(fs[0], "0:", 1);       /* ����SD�� */
//    f_mount(fs[1], "1:", 1);       /* ����FLASH. */
}

/**
 * ��һ���ļ�
 * @param drv��ָ��ú�����������������
 * @param ile_p��ָ��file_t������ָ��
 * @param path������������ͷ���ļ�·��(����S:/folder/file.txt)
 * @param mode: FS_MODE_RD, write: FS_MODE_WR, both: FS_MODE_RD | FS_MODE_WR
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
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
 * �ر��Ѵ򿪵��ļ�
 * @param  drv��ָ��ú�����������������
 * @param  file_p��ָ��file_t������ָ�롣(��lv_ufs_open��)
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
 */
static lv_fs_res_t fs_close (lv_fs_drv_t * drv, void * file_p)
{
    f_close(file_p);
    return LV_FS_RES_OK;
}

/**
 * �Ӵ򿪵��ļ��ж�ȡ����
 * @param drv��ָ��ú�����������������
 * @param file_p��ָ��file_t������ָ�롣
 * @param buf��ָ�룬ָ��洢�����ݵ��ڴ��
 * @param btr��Ҫ��ȡ���ֽ���
 * @param br��ʵ�ʶ��ֽ���(�ֽڶ�)
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
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
 * д���ļ�
 * @param  drv��   ָ��ú�����������������
 * @param  file_p��ָ��file_t������ָ��
 * @param  buf��   ָ�룬ָ��һ������Ҫд���ֽڵĻ�����
 * @param  btw��   Ҫд��btr�ֽ���
 * @param  br��    ʵ��д����ֽ���(��д����ֽ���)�����δʹ��NULL��
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
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
 * ���ö�дָ�롣�����Ҫ����������չ�ļ���С��
 * @param  drv��   ָ��ú�����������������
 * @param  file_p��ָ��file_t������ָ�롣(ʹ��lv_ufs_open��)
 * @param  pos��   ��дָ�����λ��
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
 */
static lv_fs_res_t fs_seek (lv_fs_drv_t * drv, void * file_p, uint32_t pos)
{
    f_lseek(file_p, pos);
    return LV_FS_RES_OK;
}

/**
 * ����һ���ļ��ֽڵĴ�С
 * @param  drv��   ָ��ú�����������������
 * @param  file_p��ָ��file_t������ָ��
 * @param  size��  ָ��һ���洢��С�ı���
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
 */
static lv_fs_res_t fs_size (lv_fs_drv_t * drv, void * file_p, uint32_t * size_p)
{
    (*size_p) = f_size(((file_t *)file_p));
    return LV_FS_RES_OK;
}

/**
 * ���ض�дָ���λ��
 * @param  drv��   ָ��ú�����������������
 * @param  file_p��ָ��file_t������ָ��
 * @param  pos_p�� ���ڴ洢�����ָ��
 * @return LLV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
 */
static lv_fs_res_t fs_tell (lv_fs_drv_t * drv, void * file_p, uint32_t * pos_p)
{
    *pos_p = f_tell(((file_t *)file_p));
    return LV_FS_RES_OK;
}

/**
 * ɾ���ļ�
 * @param  drv�� ָ��ú�����������������
 * @param  path��Ҫɾ�����ļ���·��
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
 */
static lv_fs_res_t fs_remove (lv_fs_drv_t * drv, const char *path)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * ��ȡ�ļ�
 * @param  drv��   ָ��ú�����������������
 * @param  file_p��ָ��'ufs_file_t'������ָ�롣(��lv_fs_open��)
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
 */
static lv_fs_res_t fs_trunc (lv_fs_drv_t * drv, void * file_p)
{
    f_sync(file_p); /*If not syncronized fclose can write the truncated part*/
    f_truncate(file_p);
    return LV_FS_RES_OK;
}

/**
 * ������һ���ļ�
 * @param  drv    ��ָ��ú�����������������
 * @param  oldname��������
 * @param  newname��������
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
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
 * ��ȡ��������Ŀ��д�С���ܴ�С(kB)
 * @param  drv��    ָ��ú�����������������
 * @param  total_p���洢�ܴ�С��ָ��[kB]
 * @param  free_p�� ָ�룬���ڴ洢���д�С[kB]
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
 */
static lv_fs_res_t fs_free (lv_fs_drv_t * drv, uint32_t * total_p, uint32_t * free_p)
{
    lv_fs_res_t res = LV_FS_RES_NOT_IMP;

    /* Add your code here*/

    return res;
}

/**
 * ��Ŀ¼
 * @param  drv��    ָ��ú�����������������
 * @param  rddir_p��ָ��'lv_fs_dir_t'������ָ��
 * @param  path��   Ŀ¼·��
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
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
 * ��Ŀ¼�ж�ȡ��һ���ļ�����Ŀ¼���ԡ�/����ͷ
 * @param  drv��    ָ��ú�����������������
 * @param  rddir_p��ָ���ʼ���ġ�lv_fs_dir_t��������ָ��
 * @param  fn��     ָ�����ļ����Ļ�������ָ��
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
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
 * �ر�Ŀ¼��ȡ
 * @param  drv��    ָ��ú�����������������
 * @param  rddir_p��ָ���ʼ���ġ�lv_fs_dir_t��������ָ��
 * @return LV_FS_RES_OK: �ɹ�, LV_FS_RES_NOT_IMP��ʧ��
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
