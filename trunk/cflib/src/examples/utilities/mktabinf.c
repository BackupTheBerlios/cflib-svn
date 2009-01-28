/**
 * mktabinf.c - make HTML table input template for CFLIB demo prog fill input
 * (c) 1996-2008 Stefan Habermehl
 * Synopsis: mktabinf [<rows>] [<cols>]
 */

#include <stdio.h>
#include <stdlib.h>

int main( int argc, char **argv )
{
	int i, j, cols, rows;
	FILE *ftmp;

	if( argc < 2 ) rows = 1;
	else rows = atoi( argv[1] );
	if( argc < 3 ) cols = 1;
	else cols = atoi( argv[2] );

	ftmp = fopen( "table.htl", "w" );

	fprintf( ftmp, "<table border=$(TBORDER) width=$(TWIDTH) colspec=$(TCOLSPEC) cellspacing=$(TSPACING) cellpadding=$(TPADDING)>\n" );
	fprintf( ftmp, "<caption>$(TCAPTION)</caption>\n" );

	for( i = 0; i <= rows; i++ ){

		for( j = 1; j <= cols; j++ ){

			if( argc > 3 ) printf( "TAB_%d_%d = \n", i, j ); /* For config file */

			if(j==1) fprintf( ftmp, "<tr $(TALLTRATTR)>" );
			if(i==0) fprintf( ftmp, "<th>$(TAB_0_%d)</th>", j );
			else fprintf( ftmp, "<td $(TALLTDATTR)>$(TAB_%d_%d)</td>",i,j );
			if(j==cols) fprintf( ftmp, "</tr>\n" );
		}
	}
	fprintf( ftmp, "</table>\n" );
	fclose( ftmp );
	return 0;
}
