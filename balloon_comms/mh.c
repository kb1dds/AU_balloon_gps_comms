/* Maidenhead locator function 
 * 
 * Copyright (c) 2022, Michael Robinson
 * This work is licensed under a Creative Commons Attribution 4.0 International License [http://creativecommons.org/licenses/by/4.0/]
 */
#include <stdio.h>
#include <math.h>

/* Convert degrees latitude N, degrees longitude E to grid square
 * 
 * Input: lat,lon = location to convert (degrees)
 * Output: grid = character array for grid square (at least 7 characters)
 * Returns: 0 if success, nonzero if location out of range
 */
int latlon2grid(double lat, double lon, char *grid);

int grid2latlon(char *grid, double *lat, double *lon);

int latlon2grid(double lat, double lon, char *grid){
  double tlat, tlon;
  char lookup;

  /* Residuals */
  tlat=lat;
  tlon=lon;

  grid[0]=floor((tlon+180)/20)+'A';
  tlon=tlon-floor(tlon/20)*20;

  grid[1]=floor((tlat+90)/10)+'A';
  tlat=tlat-floor(tlat/10)*10;

  grid[2]=floor(tlon/2)+'0';
  tlon=tlon-floor(tlon/2)*2;

  grid[3]=floor(tlat)+'0';
  tlat=tlat-floor(tlat);

  grid[4]=floor(tlon*12)+'a';
  grid[5]=floor(tlat*24)+'a';
  grid[6]=0;
}

/* Convert grid square to degrees latitude N, degrees longitude E
 * 
 * Input: grid = character array for grid square (null terminated)
 * Output: lat,lon = location (degrees)
 * Returns: 0 if success, nonzero if location out of range
 */
int grid2latlon(char *grid, double *lat, double *lon){
 
  *lat = (double)(grid[1]-'A')*10.0-90.0+(double)(grid[3]-'0')+(grid[5]-'a')/24.0;
  *lon = (double)(grid[0]-'A')*20.0-180.0+(double)(grid[2]-'0')*2.0+(grid[4]-'a')/12.0;
}

int main(){
  double lat,lon;
  char grid[10];

  latlon2grid( 39, -77, grid);
  grid2latlon( grid, &lat, &lon );
  printf("Grid at 39, -77 = %s = %g, %g\n", grid, lat, lon);

  grid2latlon( "FN31pr", &lat, &lon );
  latlon2grid( lat, lon, grid );
  printf("Location of FN31pr = %g N, %g E = %s\n", lat, lon, grid);
}
