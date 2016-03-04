#include "Save.h"
#include "simple_logger.h"
#include "stdio.h"
void playerSave(PlayerSave *ps, Events *events, char *filename)
{
	FILE *fp;
	if(!ps)return;
	fp = fopen(filename, "wb");

	if(fp)
	{
		return;
	}
	fwrite(ps,sizeof(PlayerSave),1,fp);

	fwrite(events,sizeof(Events),ps->eventNum,fp);
	fclose(fp);
}
void PlayerLoad(PlayerSave *ps, Events **events, char *filename)
{
	FILE *fp;

	if(!ps)return;
	if(!events)return;

	fp = fopen(filename,"rb");

	if(!fp)
	{
		slog("ERROR: failed to open file %s", filename);
		return;
	}

	fread(ps,sizeof(PlayerSave),1,fp);
	

	if(ps->eventNum > 0)
	{
		*events = (Events *)malloc(sizeof(Events)*ps->eventNum);
		fread(*events, sizeof(Events),ps->eventNum,fp);
	}

	fclose(fp);
}