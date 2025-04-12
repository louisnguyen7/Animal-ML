/* 

© 2024 Louis Nguyen. All rights reserved. 

*/

#include "nguyenLouisHeader.h"
#include "helper.h"

int main(int argc, char *argv[])
{
    struct Animal dataZoo[NUM_SAMPLES];
    float euclideanDistance = 0;
    int hammingDistance = 0;
    float jaccardSimilarity = 0;
    int choice = 0;
    int k = 5;  // made k = 5 to make it easy to test functions
    int whichDistanceFunction = 0, kNearestNeighbors [NUM_SAMPLES];
    int i = 0, j = 0;
    int predictedClass = 0;
    struct Animal testData[NUM_TEST_DATA];
    float accuracy = 0;
    int fileRead = 0;

    // test vectors for task 2
    int vector1[NUM_FEATURES] = {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1};
    int vector2[NUM_FEATURES] = {1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 4, 0, 0, 1};

    // test sample for tasks 3 and 4
    int newSample[NUM_FEATURES] = {1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1};

    do
    {   
        printf("\nHere is the menu - enter a number between 1 and 5\n");
        printf("1. Read the file (must be executed first) \n");
        printf("2. Distance calculations \n");
        printf("3. Find K nearest neighbors \n");
        printf("4. Predict class \n");
        printf("5. Find accuracy \n");
        printf("\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            // call task 1
            readFromFile(argv[1], dataZoo);

            for (i = 0; i < NUM_SAMPLES; i++)
            {
                printf("%s ", dataZoo[i].animalName);
                
                for (j = 0; j < NUM_FEATURES; j++)
                {
                    printf("%d ", dataZoo[i].features[j]);
                }

                printf("%d", dataZoo[i].classLabel);
                
                printf("\n");
            }

            // ensures that the user reads the file first before doing other choices
            fileRead = 1;
        }

        else if (fileRead == 0)
        {
            // exits the program when file is not read
            return 0;
        }

        else if (choice == 2)
        {
            // call task 2
            distanceFunctions(vector1, vector2, &euclideanDistance, &hammingDistance, &jaccardSimilarity);
            printf("Euclidean Distance: %f\n", euclideanDistance);
            printf("Hamming Distance: %d\n", hammingDistance);
            printf("Jaccard Similarity: %f\n", jaccardSimilarity);
        }

        else if (choice == 3)
        {
            // calling function for each distance function
            whichDistanceFunction = 1;
            findKNearestNeighbors (dataZoo, newSample, k, whichDistanceFunction, kNearestNeighbors);
            printf("Nearest neighbors for the new sample with Euclidean Distance: ");
            for (i = 0; i < k; i++)
            {
                printf("%d ", kNearestNeighbors[i]);
            }

            printf("\n");

            whichDistanceFunction = 2;
            findKNearestNeighbors (dataZoo, newSample, k, whichDistanceFunction, kNearestNeighbors);
            printf("Nearest neighbors for the new sample with Hamming Distance: ");
            for (i = 0; i < k; i++)
            {
                printf("%d ", kNearestNeighbors[i]);
            }

            printf("\n");
            
            whichDistanceFunction = 3;
            findKNearestNeighbors (dataZoo, newSample, k, whichDistanceFunction, kNearestNeighbors);
            printf("Nearest neighbors for the new sample with Jaccard Similarity: ");
            for (i = 0; i < k; i++)
            {
                printf("%d ", kNearestNeighbors[i]);
            }

            printf("\n");
        }

        else if (choice == 4)
        {
            // printing out the output for the desired distance function
            whichDistanceFunction = 1;
            // call function
            predictedClass = predictClass(dataZoo, newSample, whichDistanceFunction, k);
            printf("The predicted class with Euclidean Distance is: %d\n", predictedClass);

            printf("\n");

            whichDistanceFunction = 2;
            // call function
            predictedClass = predictClass(dataZoo, newSample, whichDistanceFunction, k);
            printf("The predicted class with Hamming Distance: %d\n", predictedClass);

            printf("\n");

            whichDistanceFunction = 3;
            // call function
            predictedClass = predictClass(dataZoo, newSample, whichDistanceFunction, k);
            printf("The predicted class with Jaccard Similarity: %d\n", predictedClass);

            printf("\n");
        }

        else if (choice == 5)
        {   
            // printing out the output for the desired distance function
            whichDistanceFunction = 1;
            accuracy = findAccuracy(dataZoo, whichDistanceFunction, testData, k);
            printf("The accuracy for the test data with Euclidean Distance is %f\n", accuracy);

            printf("\n");

            whichDistanceFunction = 2;
            accuracy = findAccuracy(dataZoo, whichDistanceFunction, testData, k);
            printf("The accuracy for the test data with Hamming Distance is %f\n", accuracy);

            printf("\n");

            whichDistanceFunction = 3;
            accuracy = findAccuracy(dataZoo, whichDistanceFunction, testData, k);
            printf("The accuracy for the test data with Jaccard Similarity is %f\n", accuracy);

            printf("\n");
        }

    } while (choice >= 1 && choice <= 5);
    // exits program when any number outside the range 1-5 to chosen

    return 0;
}