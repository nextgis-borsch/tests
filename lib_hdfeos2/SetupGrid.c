#include "hdf.h"
#include "HdfEosDef.h"
#include <math.h>

/*
 * In this example we will open an HDF file and create UTM and Polar
 * Stereographic grid structures within the file.
 */


main()
{

    intn            status, i, j;
    int32           gdfid, GDid, GDid2, GDid3, dummy;
    int32           xdim, ydim, zonecode, projcode, spherecode;

    float64         projparm[16], uplft[2], lowrgt[2];
    float64         EHconvAng();
    char *ptr = NULL;

    /*
     * We first open the HDF grid file, "GridFile.hdf".  Because this file
     * does not already exist, we use the DFACC_CREATE access code in the
     * open statement.  The GDopen routine returns the grid file id, gdfid,
     * which is used to identify the file in subsequent routines in the
     * library.
     */
    /* a long filename (> 80 characters) is used to test long file names ( > 80 characters) 
       that caused problem in version 2.18
    */


    gdfid = GDopen("GridFile_created_with_hadeos_sample_file_writer_of_HDFEOS2_version_219_or_higher_release.hdf", DFACC_CREATE);
    if (gdfid == -1)
    {
      printf("\t\tError: Cannot open file \"GridFile.hdf\"\n");
      return -1;
    }

    /*
     * Create UTM Grid
     * 
     * Region is bounded by 54 E and 60 E longitude and 20 N and 30 N latitude.
     * UTM Zone 40
     * 
     * Use default spheriod (Clarke 1866 - spherecode = 0)
     * 
     * Grid into 120 bins along x-axis and 200 bins along y-axis
     *                   (approx 3' by 3' bins)
     */

    zonecode = 40;
    spherecode = 0;

    /* Upper Left and Lower Right points in meters */
    /* ------------------------------------------- */
    uplft[0] =   210584.50041;
    uplft[1] =  3322395.95445;
    lowrgt[0] =  813931.10959;
    lowrgt[1] = 2214162.53278;

    xdim = 120;
    ydim = 200;
    
    GDid = GDcreate(gdfid, "UTMGrid", xdim, ydim, uplft, lowrgt);
    if (GDid  == -1)
      {
	printf("\t\tError: Cannot create grid \"UTMGrid\"\n");
	return -1;
      }
    status = GDdefproj(GDid, GCTP_UTM, zonecode, spherecode, projparm);
    if (status == -1)
      {
	printf("\t\tError: Cannot define UTM projection for grid\"UTMGrid\"\n");
	return -1;
      }
    /* Define "Time" Dimension */
    status = GDdefdim(GDid, "Time", 10);
   if (status == -1)
      {
	printf("\t\tError: Cannot define dim \"Time\"\n");
	return -1;
      }


    /*
     * Create polar stereographic grid
     * 
     * Northern Hemisphere  (True Scale at 90 N, 0 Longitude below pole)
     * 
     * Use International 1967 spheriod (spherecode = 3)
     * 
     * Grid into 100 bins along x-axis and y-axis
     */

    spherecode = 3;

    /* Define GCTP Projection Parameters */
    /* --------------------------------- */
    for (i = 0; i < 16; i++)
	projparm[i] = 0;

    /* Set Longitude below pole & true scale in DDDMMMSSS.SSS format) */
    projparm[4] = 0.0;
    projparm[5] = 90000000.00;
    

    xdim = 100;
    ydim = 100;

    GDid2 = GDcreate(gdfid, "PolarGrid", xdim, ydim, NULL, NULL);
    if (GDid2  == -1)
      {
	printf("\t\tError: Cannot create grid \"PolarGrid\"\n");
	return -1;
      }
    status = GDdefproj(GDid2, GCTP_PS, dummy, spherecode, projparm);
    if (status == -1)
      {
	printf("\t\tError: Cannot define PS projection for grid\"PolarGrid\"\n");
	return -1;
      }
    status = GDdeforigin(GDid2, HDFE_GD_LR);
        if (status == -1)
      {
	printf("\t\tError: Cannot define origin for grid\"PolarGrid\"\n");
	return -1;
      }
    /* Define "Bands" Dimension */
    status = GDdefdim(GDid2, "Bands", 3);
   if (status == -1)
      {
	printf("\t\tError: Cannot define dim \"Bands\"\n");
	return -1;
      }


    /*
     * Create geographic (linear scale) grid
     * 
     * 0 - 15 degrees longitude,  20 - 30 degrees latitude
     *
     */
    
    xdim = 60;
    ydim = 40;

    uplft[0] = EHconvAng(0., HDFE_DEG_DMS);
    uplft[1] = EHconvAng(30., HDFE_DEG_DMS);
    lowrgt[0] = EHconvAng(15., HDFE_DEG_DMS);
    lowrgt[1] = EHconvAng(20., HDFE_DEG_DMS);
    
    GDid3 = GDcreate(gdfid, "GEOGrid", xdim, ydim, uplft, lowrgt);
    if (status == -1)
      {
	printf("\t\tError: Cannot create GEO grid\"GEOGrid\"\n");
	return -1;
      }
    status = GDdefproj(GDid3, GCTP_GEO, dummy, dummy, NULL);
    if (status == -1)
      {
	printf("\t\tError: Cannot define GEO projection for grid\"GEOGrid\"\n");
	return -1;
      }

    /*
     * We now close the grid interface with the GDdetach routine.  This step
     * is necessary to properly store the grid information within the file
     * AND SHOULD BE DONE BEFORE WRITING OR READING DATA TO OR FROM THE FIELD.
     */
    
    GDdetach(GDid);
    GDdetach(GDid2);
    GDdetach(GDid3);

    /*
     * Finally, we close the grid file using the GDclose routine.  This will
     * release the grid file handles established by GDopen.
     */

    GDclose(gdfid);

    return 0;
}
