#include "hdf.h"


/*
 * In this example we will (1) open the "GridFile" HDF file, (2) attach to
 * the "UTMGrid", and (3) write data to the "Vegetation" field.  We will
 * then attach to the "PolarGrid" and write to the "Temperature" field.
 */


main()
{

    intn            i, j, status;
    
    int32           gdfid, GDid,start[3],stride[3],edge[3];

    float32         f32=1.0;
    float32         veg[200][120], temp[100][100];


    /* Fill veg array */
    for (i=0; i<200;i++)
	for (j=0; j<120; j++)
	    veg[i][j] = 10+i;

    /* Fill temp array */
    for (i=0; i<100;i++)
	for (j=0; j<100; j++)
	    temp[i][j] = 100*i+j;

    
    /*
     * Open the HDF grid file, "GridFile.hdf".
     */

    gdfid = GDopen("GridFile_created_with_hadeos_sample_file_writer_of_HDFEOS2_version_219_or_higher_release.hdf", DFACC_RDWR);


    if (gdfid != -1)
    {

	/*
	 * Attach the "UTMGrid".
	 */
	GDid = GDattach(gdfid, "UTMGrid");

	if (GDid != -1)
	{
	  status = GDwritefield(GDid, "Vegetation", 
				NULL, NULL, NULL, veg);
	  if (status == -1)
	    {
	      printf("\t\tError: Cannot write field \"Vegetation\"\n");
	      return -1;
	    }
	  /*
	   * It will always fail
	  status = GDwritefield(GDid, "Vegetat", 
				NULL, NULL, NULL, veg);
	  if (status == -1)
	    {
	      printf("\t\tError: Cannot write field \"Vegetat\"\n");
	      printf("\t\tField \"Vegetat\" has not been defined.\n");
	    }
	  */
	  status = GDwriteattr(GDid, "float32", DFNT_FLOAT32, 1, &f32);
	  if (status == -1)
	    {
	      printf("\t\tError: Cannot write attribute \"float32\"\n");
	      return -1;
	    }
	}

	GDdetach(GDid);


	GDid = GDattach(gdfid, "PolarGrid");
	if (GDid != -1)
	{
	  status = GDwritefield(GDid, "Temperature", 
				NULL, NULL, NULL, temp);
	  if (status == -1)
	    {
	      printf("\t\tError: Cannot write field \"Temperature\"\n");
	      return -1;
	    }
	}
	GDdetach(GDid);

    }

    GDclose(gdfid);

    return 0;
}
