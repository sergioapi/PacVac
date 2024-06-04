# Vaccination Center Management System

This project, developed for the Algorithms and Data Structures course, is a C application designed to manage a vaccination center.

## Description

The aim of this project is to create a software system to efficiently manage the operations of a vaccination center. The system utilizes various data structures and algorithms to handle tasks such as patient registration, vaccine administration, and data storage.

### Features
- **Add Patient**: Allows adding a new patient to the database along with their vaccination details.
- **List Patients Alphabetically**: Prints the details of all patients in alphabetical order along with their vaccination information.
- **Delete Patient**: Enables removal of a patient from the database.
- **File Input/Output**: Reads patient data from a file specified via the command line and updates the database accordingly.
- **Update Patient Vaccination Status**: Generates a daily list of patients to be vaccinated and updates their vaccination status in the database.

## How to Use
1. Compile the program using a C compiler.
2. Execute the compiled executable file with the following command:
   ./vaccination_center -f FILE_NAME Replace `FILE_NAME` with the name of the input file containing patient data.

## File Format for Input
The input file should have the following format for each patient:
PATIENT_NAME|VACCINE_1;DOSES_VACCINE_1;MAX_DOSES_VACCINE_1;VACCINE_2;DOSES_VACCINE_2;MAX_DOSES_VACCINE_2;

## Requirements
- C Compiler

## Credits
This project was developed as part of the Algorithms and Data Structures course by **Sergio Álvarez Piñón**.

## License
This project is licensed under the [MIT License](LICENSE).

