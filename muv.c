#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void create_directory(const char *name) {
    if (mkdir(name, 0777) == -1) {
        perror("Error creating directory");
        exit(EXIT_FAILURE);
    }
}

void run_command(const char *command) {
    int ret = system(command);
    if (ret != 0) {
        fprintf(stderr, "Command failed: %s\n", command);
        exit(EXIT_FAILURE);
    }
}

void init_project(const char *project_name) {
    // Create project directory
    create_directory(project_name);

    // Change to project directory
    if (chdir(project_name) != 0) {
        perror("Error changing to project directory");
        exit(EXIT_FAILURE);
    }

    // Initialize git repository
    run_command("git init");

    // Create src directory
    create_directory("src");

    // Create a basic main.c file in src/
    FILE *file = fopen("src/main.c", "w");
    if (!file) {
        perror("Error creating main.c");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "#include <stdio.h>\n\nint main() {\n    printf(\"Hello, World!\\n\");\n    return 0;\n}\n");
    fclose(file);

    printf("Project '%s' initialized successfully.\n", project_name);
}

void run_project() {
    // Compile the program
    if (access("src/main.c", F_OK) == -1) {
        fprintf(stderr, "Error: src/main.c does not exist.\n");
        exit(EXIT_FAILURE);
    }
    printf("Compiling...\n");
    run_command("gcc src/main.c -o main");

    // Run the compiled program
    printf("Running...\n");
    run_command("./main");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "init") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: %s init <project_name>\n", argv[0]);
            return EXIT_FAILURE;
        }
        init_project(argv[2]);
    } else if (strcmp(argv[1], "run") == 0) {
        run_project();
    } else {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

