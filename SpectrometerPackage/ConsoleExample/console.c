#include "SpectrometerDriver.h"
#include "stdio.h"

int main()
{
	unsigned int detectorID = 0;

    // Initialise the library with no error callback (not recommended)
	kr_Initialise(NULL, NULL);

	// Go through every detector, list its details and take a 5 second acquisition	
    while ((detectorID = kr_GetNextDetector(detectorID)))
	{
		char nameBuffer[200];
		int nameLength = 200;
		unsigned int totalCounts = 0;

		kr_GetDeviceName(detectorID, nameBuffer, nameLength, &nameLength);
        printf ("Detector Found - %u: %s\n", detectorID, nameBuffer);

		// 5 Seconds (5000ms)
		kr_BeginDataAcquisition(detectorID, 5000, 0);
        printf("Acquiring Data...\n");
		while (kr_IsAcquiringData(detectorID))
            ;

        // Print out the total counts acquired during the acquisition
		kr_GetAcquiredData(detectorID, NULL, &totalCounts, NULL, NULL);
		printf("\nTotal Counts = %u\n", totalCounts);
	}
	
    // Cleanup the library
	kr_Destruct();
	return 0;
}
