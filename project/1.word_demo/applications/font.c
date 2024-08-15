#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_posix.h>
#define DBG_ENABLE
#define DBG_SECTION_NAME "font"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
#include <fal.h>

void read_word_line(void)
{
	int fd = open("/sdcard/3.CET4.txt",O_RDONLY);
	if (fd == -1)
	{
		LOG_E("open 3.CET4.txt failed");
		return;
	}
	char buf[512];
	int size = 0;
	while (1)
	{
		read(fd,&buf[size],1);
		if(buf[size] == '\n')
		{
			buf[size] = '\0';
			LOG_I("%s",buf);
			
			break;
		}
		else
		{
			size++;
		}
	}
	
	close(fd);
	
	
}
MSH_CMD_EXPORT(read_word_line,read_word_line);
void font_sd_to_flash(void)
{
	int fd = open("/sdcard/myFont.bin",O_RDONLY);
	if(fd == -1)
	{
		LOG_E("open myFont.bin failed");
		return;
	}
	LOG_I("open my_font.bin success");
	const struct fal_partition * font = fal_partition_find("font");
	if(font == RT_NULL)
	{
		LOG_E("find font failed");
		return;
	}
	char * buf = rt_malloc(4096);
	if(buf == RT_NULL)
	{
		LOG_E("malloc failed");
		close(fd);
		return;
	}
	LOG_I("malloc success");

	int count = 0;
	while (1)
	{
		int ret = read(fd,buf,4096);
		fal_partition_write(font,count,buf,ret);
		count += ret;
		if (ret != 4096)
		{
			break;
		}
		
	}
	LOG_I("font write success");
	LOG_I("font size = %d",count);

	rt_free(buf);
	close(fd);
}
MSH_CMD_EXPORT(font_sd_to_flash,font_sd_to_flash);



void file_test(void)
{

	int fd = dfs_mkfs("elm","filesystem");
	if(fd == -1)
	{
		LOG_E("mkfs failed");
		return;
	}
	
	
	fd = dfs_mount("filesystem","/fal","elm",0,0);
	if(fd == -1)
	{
		LOG_E("mount failed");
		return;
	}
	LOG_I("mount success");
	fd = open("/fal/text.txt",O_WRONLY | O_CREAT);
	if(fd == -1)
	{
		LOG_E("open failed");
		return;
	}
	LOG_I("open success");
	char buf[100] = "hello world";
	write(fd,buf,rt_strlen(buf));
	
	LOG_I("write success");
	close(fd);

}
MSH_CMD_EXPORT(file_test,file_test);


void fal_font_erase(void)
{
	const struct fal_partition * font = fal_partition_find("font");
	if(font == RT_NULL)
	{
		LOG_E("find font failed");
		return;
	}

	int size = fal_partition_erase_all(font);
	LOG_I("erase all size = %d",size);
}
MSH_CMD_EXPORT(fal_font_erase,fal_font_erase);
