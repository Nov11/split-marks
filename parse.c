int
main (int argc, char **argv) 
{
  if (argc != 2)
    
    {
      printf ("Usage: \n");
      return 1;
    }
  char szFileBuff[1024] = { 0 }, szBuff[1024];
  FILE * fp;
  char szName[64] = { 0 }, szId[64] =
  {
  0}, szSex[64] =
  {
  0}, szAge[64] =
  {
  0};
  char *lFirst, *lEnd;
   if ((fp = fopen (argv[1], "r")) == NULL)
    
    {
      printf ("fopen %s file error!\n", argv[1]);
      return 0;
    }
  while (fgets (szFileBuff, 1023, fp))
    
    {
      if ((lFirst = strstr (szFileBuff, "")) != NULL)
	
	{
	  lEnd = strstr (lFirst + 1, "");
	  memcpy (szName, lFirst + 6, lEnd - lFirst - 6);
	}
      if ((lFirst = strstr (szFileBuff, "")) != NULL)
	
	{
	  lEnd = strstr (lFirst + 1, "");
	  memcpy (szId, lFirst + 4, lEnd - lFirst - 4);
	}
      if ((lFirst = strstr (szFileBuff, "")) != NULL)
	
	{
	  lEnd = strstr (lFirst + 1, "");
	  memcpy (szSex, lFirst + 5, lEnd - lFirst - 5);
	}
      if ((lFirst = strstr (szFileBuff, "")) != NULL)
	
	{
	  lEnd = strstr (lFirst + 1, "");
	  memcpy (szAge, lFirst + 5, lEnd - lFirst - 5);
	}
      sprintf (szBuff, "name:%s;id:%s;sex:%s;age:%s", szName, szId,
		szSex, szAge);
      printf ("buff[%s]\n", szBuff);
    }
   fclose (fp);
  return 0;
}
