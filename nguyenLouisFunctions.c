/* 

© 2024 Louis Nguyen. All rights reserved. 

*/

#include "nguyenLouisHeader.h"
#include "helper.h"

int readFromFile(char fName[30], struct Animal dataZoo[NUM_SAMPLES])
{
    FILE *readFile;
    int i = 0;
    int count = 0;

    readFile = fopen(fName, "r");

    // check if file exists
    if (readFile == NULL)
    {
        return -1;
    }

    else
    {
        while (!feof(readFile))
        {
            // use fscanf to collect the data of the file
            // read the animal name (e.g. aardvark)
            fscanf(readFile, "%s", dataZoo[count].animalName);

            // read the features (an array) of each animal (e.g. 1 0 0 1 0 0 1 1 1 1 0 0 4 0 0 1)
            for (i = 0; i < NUM_FEATURES; i++)
            {
                fscanf(readFile, "%d", &dataZoo[count].features[i]);
            }

            // read the class label of each animal (e.g. 1)
            fscanf(readFile, "%d", &dataZoo[count].classLabel);

            count++;
        }
    }

    fclose(readFile);

    return 1;
}

void distanceFunctions(int vector1[NUM_FEATURES],
                       int vector2[NUM_FEATURES],
                       float *euclideanDistance,
                       int *hammingDistance,
                       float *jaccardSimilarity)
{
    int i = 0;
    float ED = 0;
    int count0 = 0, count1 = 0;
    float numerator = 0, denominator = 0;

    // resetting after each call
    *euclideanDistance = 0;
    *hammingDistance = 0;
    *jaccardSimilarity = 0;

    // calculate euclidean distance
    for (i = 0; i < NUM_FEATURES; i++)
    {
        ED += (pow(vector1[i] - vector2[i], 2));
    }

    *euclideanDistance = sqrt(ED);

    // calculate hamming distance
    for (i = 0; i < NUM_FEATURES; i++)
    {
        if (vector1[i] != vector2[i])
        {
            (*hammingDistance)++;
        }
    }
            
    // calculate jaccard similarity
    // calculate number of 1-1 matches
    for (i = 0; i < NUM_FEATURES; i++)
    {
        if (vector1[i] == 1 && vector2[i] == 1)
        {
            count1++;
        }
    }

    // calculate number of 0-0 matches
    for (i = 0; i < NUM_FEATURES; i++)
    {
        if (vector1[i] == 0 && vector2[i] == 0)
        {
            count0++;
        }
    }

    /* calculate the formula for the denominator
    (number of features in vector 1 or 2 - number of 0-0 matches) */
    denominator = NUM_FEATURES - count0;
    numerator = count1;

    *jaccardSimilarity = numerator / denominator;
}

/* I used GeeksforGeeks to get the code for the sorting algorithm. 
Cite: https://www.geeksforgeeks.org/bubble-sort-algorithm/ */
void findKNearestNeighbors (struct Animal dataZoo [NUM_SAMPLES], int newSample [NUM_FEATURES], int k, int whichDistanceFunction, int kNearestNeighbors [NUM_SAMPLES])
{
    float euclideanDistance = 0;
    int hammingDistance = 0;
    float jaccardSimilarity = 0;
    int i = 0, j = 0;
    float tempDistance = 0;
    int tempIndex = 0;

    // 2-D array to store the distances of each animal to the new sample
    float distances[NUM_SAMPLES][2]; // [Distances of each animal][Indices]

    // k-nearest-algorithm
    for (i = 0; i < NUM_SAMPLES; i++)
    {
        // call distance functions to compare the animal features in the data to the new sample
        distanceFunctions(dataZoo[i].features, newSample, &euclideanDistance, &hammingDistance, &jaccardSimilarity);

        // populating the 2-D array distances with data based on the distance function
        if (whichDistanceFunction == 1)
        {
            // save the new calculated distances and the indices for each animal in the 2-D array
            distances[i][0] = euclideanDistance;
            distances[i][1] = i;
        }

        else if (whichDistanceFunction == 2)
        {
            distances[i][0] = hammingDistance;
            distances[i][1] = i;
        }

        else if (whichDistanceFunction == 3)
        {
            distances[i][0] = jaccardSimilarity;
            distances[i][1] = i;
        }
    }

    // sort distances algorithm
    if (whichDistanceFunction == 1 || whichDistanceFunction == 2)
    {
        // sort in increasing order (lowest values to greatest)
        for (i = 0; i < NUM_SAMPLES - 1; i++)
        {
            for (j = 0; j < NUM_SAMPLES - i - 1; j++)
            {
                if (distances[j][0] > distances[j + 1][0])
                {
                    // swapping distances
                    tempDistance = distances[j][0];
                    distances[j][0] = distances[j + 1][0];
                    distances[j + 1][0] = tempDistance;

                    // swapping indices
                    tempIndex = distances[j][1];
                    distances[j][1] = distances[j + 1][1];
                    distances[j + 1][1] = tempIndex; 
                }
            }
        }
    }

    else if (whichDistanceFunction == 3)
    {
        // sort in decreasing order (highest values first)
        for (i = 0; i < NUM_SAMPLES - 1; i++)
        {
            for (j = 0; j < NUM_SAMPLES - i - 1; j++)
            {
                if (distances[j][0] < distances[j + 1][0])
                {
                    // swapping distances
                    tempDistance = distances[j][0];
                    distances[j][0] = distances[j + 1][0];
                    distances[j + 1][0] = tempDistance;

                    // swapping indices
                    tempIndex = distances[j][1];
                    distances[j][1] = distances[j + 1][1];
                    distances[j + 1][1] = tempIndex;
                }
            }
        }
    }

    // save the top K indices in the kNearestNeighbors array
    for (i = 0; i < k; i++)
    {
        kNearestNeighbors[i] = distances[i][1];
    }
}

/* I used GeeksforGeeks to get the code for the sorting algorithm. 
Cite: https://www.geeksforgeeks.org/bubble-sort-algorithm/ */
int predictClass (struct Animal dataZoo [NUM_SAMPLES], int newSample [NUM_FEATURES], int whichDistanceFunction, int k)
{
    int kNearestNeighbors [NUM_SAMPLES];
    int classData[NUM_CLASSES][2]; // [ClassLabel][Frequency]
    int tempFreq = 0;
    int predictedClass = 0;
    int count = 0;
    int i = 0, j = 0;

    // call function
    findKNearestNeighbors(dataZoo, newSample, k, whichDistanceFunction, kNearestNeighbors);

    /* this loop will find the class labels of the k neighbors for the new sample and 
        then use those values to find the frequency. The classData array is used to keep track of 
            distinct classLabels found in k neighbors */
    for (i = 0; i < k; i++)
    {
        for (j = 0; j < count; j++)
        {
            if (dataZoo[kNearestNeighbors[i]].classLabel == classData[j][0])
            {
                // this will count how frequent a number shows up
                classData[j][1]++;
                /* if the number is already in classData, then break out of the loop. This ensures there are no duplicates
                    in the array when checking for frequencies */
                break;
            }
        }

        // if is not in classData, then this means that this number is distinct and will be added to the array
        if (j == count)
        {
            classData[count][0] = dataZoo[kNearestNeighbors[i]].classLabel;
            /* this is set to 1 to indicate that the number has appeared. 
                It will then iterate when it shows up again in the nexted for loop */
            classData[count][1] = 1;
            count++;
        }
    }
    
    // sort all the frequencies from greatest to least to print out the predicted label
    for (i = 0; i < count - 1; i++)
        {
            for (j = 0; j < count - i - 1; j++)
            {
                // sorting the frequencies from greatest to least
                if (classData[j][1] < classData[j + 1][1])
                {
                    // swapping labels
                    tempFreq = classData[j][0];
                    classData[j][0] = classData[j + 1][0];
                    classData[j + 1][0] = tempFreq;

                    // swapping frequencies
                    tempFreq = classData[j][1];
                    classData[j][1] = classData[j + 1][1];
                    classData[j + 1][1] = tempFreq;
                }
            }
        }

    /* check if there are numbers that have the same frequency, take the smallest number and return it
        if not, then return the number with the highest frequency which is stored in classData[0][0] from the sorting algorithm */
    for (i = 0; i < count; i++)
    {
        if (classData[i][1] == classData[0][1])
        {
            // if value i is less than value stored in [0][0] of the array, then swap it and return the smallest value
            if (classData[i][0] < classData[0][0])
            {
                classData[0][0] = classData[i][0];
            }
        }
    }

    // return the predicted class
    predictedClass = classData[0][0];

    return predictedClass;
}

float findAccuracy (struct Animal dataZoo [NUM_SAMPLES], int whichDistanceFunction, struct Animal testData [NUM_TEST_DATA], int k)
{
    float correctPredictions = 0;
    float totalPredictions = NUM_TEST_DATA;
    int predictedClass = 0;
    int i = 0;
    float accuracy = 0;

    // read csv file
    readTestData(testData);

    // this loop will check for the number of correct predictions with the number of animals in the given csv file
    for (i = 0; i < NUM_TEST_DATA; i++)
    {
        // using predictClass function to predict the classes for each animal in the csv file
        predictedClass = predictClass(dataZoo, testData[i].features, whichDistanceFunction, k);

        if (predictedClass == testData[i].classLabel)
        {
            correctPredictions++;
        }
    }

    // calculate and return the accuracy of predictions
    accuracy = correctPredictions / totalPredictions;
    
    return accuracy;
}

/* Helper function (will read the contents in the csv file) */
int readTestData (struct Animal testData [NUM_TEST_DATA])
{
    FILE *fptr;
    int i = 0;
    int count = 0;
    char animalData[100];
    char *token;

    fptr = fopen("testData.csv", "r");

    // check if file exists
    if (fptr == NULL)
    {
        return -1;
    }

    else
    {
        while (!feof(fptr))
        {
            fgets(animalData, 100, fptr);
            // the variable 'token' is where we store the parsed data in each line in the file
            token = strtok(animalData, ",");

            // if token does not equal the null terminator, it will keep populating the struct with animal names
            // e.g. calf, => strtok sees a comma so it stops and stores the animal name into the struct
            if (token != NULL)
            {
                 strcpy(testData[count].animalName, token);
            }

            /* e.g. 1,0,0,1,0,0,0,1,1,1,0,0,4,1,1,1, => everytime it sees a comma it stops and stores that integer into the struct.
                this repeats for NUM_FEATURES amount of times to collect all the animal features */
            for (i = 0; i < NUM_FEATURES; i++)
            {
                token = strtok(NULL, ",");

                if (token != NULL)
                {
                    sscanf(token, "%d", &testData[count].features[i]);
                }
            }

            // initialize the token to collect the class labels that are at the end of each animal data
            token = strtok(NULL, ",");

            /* e.g. after the loop that collects the features of the animal, now this if statement collects the
             class label of each animal */
            if (token != NULL)
            {
                sscanf(token, "%d", &testData[count].classLabel);
            }

            count++;
        }
    }

    fclose(fptr);

    return 1;
}