#include "hdf.h"


/*
 * In this example we will (1) open the "GridFile" HDF file, (2) attach to
 * the "UTMGrid", and (3) read data from the "Vegetation" field.
 */


main()
{

    intn            i, j, status;
    
    int32           gdfid, GDid;

    float32         f32=1.0;
    float32         veg[200][120];


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
	  status = GDreadfield(GDid, "Vegetation", 
				NULL, NULL, NULL, veg);
	  if (status == -1)
	    {
	      printf("\t\tError: Cannot read field \"Vegetation\"\n");
	      return -1;
	    }
          status = GDreadattr(GDid, "float32", &f32);
	  if (status == -1)
	    {
	      printf("\t\tError: Cannot read attribute \"float32\"\n");
	      return -1;
	    }
	}
    }

    GDdetach(GDid);

    GDclose(gdfid);

    return 0;
}
