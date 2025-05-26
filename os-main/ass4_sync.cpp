#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// Shared resource: number of available books in the library
int availableBooks;
pthread_rwlock_t rwlock;
pthread_mutex_t writeMutex;

// Student thread function with synchronization (Readers)
// void* reader_sync(void* arg) {
//     int student_id = *(int*)arg;
//     usleep(rand() % 50000);  // Simulate random delay

//     pthread_rwlock_rdlock(&rwlock);  // Acquire read lock
//     printf("Student %d is checking the number of available books.\n", student_id);
//     printf("Student %d: Available books = %d\n", student_id, availableBooks);
//     pthread_rwlock_unlock(&rwlock);  // Release read lock

//     free(arg);
//     return NULL;
// }

// Librarian thread function with synchronization (Writers)
// void* writer_sync(void* arg) {
//     int librarian_id = *(int*)arg;
//     usleep(rand() % 50000);  // Simulate random delay

//     pthread_mutex_lock(&writeMutex);  // Additional mutex for writers
//     pthread_rwlock_wrlock(&rwlock);   // Acquire write lock
//     printf("Librarian %d is attempting to issue a book.\n", librarian_id);

//     if (availableBooks > 0) {
//         availableBooks--;  // Issue a book (reduce available count by 1)
//         printf("Librarian %d: Book issued successfully. Remaining books = %d\n", librarian_id, availableBooks);
//     } else {
//         printf("Librarian %d: Cannot issue book, no books are available.\n", librarian_id);
//     }

//     pthread_rwlock_unlock(&rwlock);   // Release write lock
//     pthread_mutex_unlock(&writeMutex);  // Release mutex

//     free(arg);
//     return NULL;
// }

// Student thread function without synchronization (Readers)
void* reader_no_sync(void* arg) {
    int student_id = *(int*)arg;
    usleep(rand() % 10000);  // Simulate random delay

    printf("Student %d is checking the number of available books.\n", student_id);
    sched_yield();  // Force a potential context switch
    int temp = availableBooks;  // Read operation without synchronization
    usleep(rand() % 5000);  // Simulate processing time
    printf("Student %d: Available books = %d\n", student_id, temp);

    free(arg);
    return NULL;
}

// Librarian thread function without synchronization (Writers)
void* writer_no_sync(void* arg) {
    int librarian_id = *(int*)arg;
    usleep(rand() % 10000);  // Simulate random delay

    printf("Librarian %d is attempting to issue a book.\n", librarian_id);
    int temp = availableBooks;  // Read current value
    sched_yield();  // Force a potential context switch

    if (temp > 0) {
        usleep(rand() % 5000);  // Simulate processing time
        availableBooks = temp - 1;  // Write operation without synchronization
        printf("Librarian %d: Book issued successfully. Remaining books = %d\n", librarian_id, availableBooks);
    } else {
        printf("Librarian %d: Cannot issue book, no books are available.\n", librarian_id);
    }

    free(arg);
    return NULL;
}

// // Run synchronized version
// void run_synchronized(int number_of_students, int number_of_librarians) {
//     printf("\nRunning with synchronization...\n\n");

//     // Initialize locks
//     pthread_rwlock_init(&rwlock, NULL);
//     pthread_mutex_init(&writeMutex, NULL);

//     pthread_t threads[number_of_students + number_of_librarians];
//     int thread_count = 0;

//     // Create reader threads (Students)
//     for (int i = 0; i < number_of_students; i++) {
//         int* id = (int*)malloc(sizeof(int));
//         *id = i + 1;
//         pthread_create(&threads[thread_count++], NULL, reader_sync, id);
//     }

//     // Create writer threads (Librarians)
//     for (int i = 0; i < number_of_librarians; i++) {
//         int* id = (int*)malloc(sizeof(int));
//         *id = i + 1;
//         pthread_create(&threads[thread_count++], NULL, writer_sync, id);
//     }

//     // Wait for all threads to complete
//     for (int i = 0; i < thread_count; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     // Clean up locks
//     pthread_rwlock_destroy(&rwlock);
//     pthread_mutex_destroy(&writeMutex);
// }

// Run non-synchronized version
void run_non_synchronized(int number_of_students, int number_of_librarians) {
    printf("\nRunning without synchronization...\n\n");

    pthread_t threads[number_of_students + number_of_librarians];
    int thread_count = 0;

    // Create both reader and writer threads, interleaving them
    for (int i = 0; i < number_of_students || i < number_of_librarians; i++) {
        if (i < number_of_librarians) {
            int* w_id = (int*)malloc(sizeof(int));
            *w_id = i + 1;
            pthread_create(&threads[thread_count++], NULL, writer_no_sync, w_id);
        }

        if (i < number_of_students) {
            int* r_id = (int*)malloc(sizeof(int));
            *r_id = i + 1;
            pthread_create(&threads[thread_count++], NULL, reader_no_sync, r_id);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    srand(time(NULL));  // Initialize random seed
    // int choice;
    int number_of_students, number_of_librarians, initial_books;

        printf("\nEnter the number of available books: ");
        scanf("%d", &initial_books);

        printf("Enter the number of students (readers): ");
        scanf("%d", &number_of_students);

        printf("Enter the number of librarians (writers): ");
        scanf("%d", &number_of_librarians);

        availableBooks = initial_books;
        run_non_synchronized(number_of_students, number_of_librarians);

    // do {
    //     printf("\nLibrary Book Management System (Reader-Writer Problem)\n");
    //     printf("1. Run with synchronization\n");
    //     printf("2. Run without synchronization\n");
    //     printf("0. Exit program\n");
    //     printf("Enter your choice: ");
    //     scanf("%d", &choice);

    //     if (choice == 0) {
    //         printf("\nExiting program.\n");
    //         break;
    //     }

    //     int number_of_students, number_of_librarians, initial_books;

    //     printf("\nEnter the number of available books: ");
    //     scanf("%d", &initial_books);

    //     printf("Enter the number of students (readers): ");
    //     scanf("%d", &number_of_students);

    //     printf("Enter the number of librarians (writers): ");
    //     scanf("%d", &number_of_librarians);

    //     availableBooks = initial_books;

    //     switch (choice) {
    //         case 1:
    //             run_synchronized(number_of_students, number_of_librarians);
    //             break;
    //         case 2:
    //             run_non_synchronized(number_of_students, number_of_librarians);
    //             break;
    //         default:
    //             printf("\nInvalid choice. Please enter 0, 1, or 2.\n");
    //     }

    // } while (choice != 0);

    return 0;
}