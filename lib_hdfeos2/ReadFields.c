#include "hdf.h"


/*
 * In this example we will (1) open the "SwathFile" HDF file, (2) attach to
 * the "Swath1" swath, and (3) read data from the "Longitude" field.
 * 
 * Unlike the WriteField routine, we will read the field all at once.
 */


main()
{

    intn            status, i, j, k, start[2],stride[2],count[2];
    int32           swfid, SWid, attr[4];

    float32         lng[20][10];
    /* Allocate space for the longitude and spectral data */


    /*
     * Open the HDF swath file, "SwathFile.hdf".
     */

    swfid = SWopen("SwathFile_created_with_hadeos_sample_file_writer_of_HDFEOS2_version_219_or_higher_release.hdf", DFACC_READ);
    if (swfid == -1)
      {
	printf("\t\tError: Cannot open file \"SwathFile.hdf\"\n");
	return -1;
      }

    if (swfid != -1)
    {

	/*
	 * Attach the "Swath1" swath.
	 */

	SWid = SWattach(swfid, "Swath1");
	if (SWid == -1)
	  {
	    printf("\t\tError: Cannot attach swath \"Swath1\"\n");
	    return -1;
	  }
	if (SWid != -1)
	{
	    /* Read the entire longitude field */
	    status = SWreadfield(SWid, "Longitude", NULL, NULL, NULL, lng);
	    if (status == -1)
	      {
		printf("\t\tError: Cannot read field \"Longitude\"\n");
		return -1;
	      }
	    /* Print field */
	    for (i = 0; i < 20; i++)
		for (j = 0; j < 10; j++)
		    printf("i j Longitude: %d %d %f\n", 
			   i, j, lng[i][j]);


	    /* Read User Attribute */
	    status = SWreadattr(SWid, "TestAttr", attr);
	    if (status == -1)
	      {
		printf("\t\tError: Cannot read attribute \"TestAttr\"\n");
		return -1;
	      }
	    for (i=0;i<4;i++)
		printf("Attribute Entry %d: %d\n",i+1,attr[i]);

	}
    }
    status = SWdetach(SWid);
    if (status == -1)
      {
	printf("\t\tError: Cannot detach SWid\n");
	return -1;
      }
    status = SWclose(swfid);
    if (status == -1)
      {
	printf("\t\tError: Cannot close hdf file.\n");
	return -1;
      }
    HEprint(stdout,0);
    
    return 0;
}
