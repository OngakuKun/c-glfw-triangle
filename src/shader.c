#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

static char *read_file_to_string(const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", path);
        return NULL;
    }

    // Seek to end to determine size
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }

    long length = ftell(file);
    rewind(file);

    if (length < 0) {
        fclose(file);
        return NULL;
    }

    // Allocate memory (+1 for null terminator)
    char *buffer = malloc((size_t)length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    size_t read_len = fread(buffer, 1, (size_t)length, file);
    buffer[read_len] = '\0';

    fclose(file);

    return buffer;
}

static GLuint load_shader(const char *path, GLenum shader_type) {
    char *source = read_file_to_string(path);
    if (!source)
        return 0;

    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const GLchar *const *)&source, NULL);
    glCompileShader(shader);

    free(source);

    // Check for compile errors
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        fprintf(stderr, "Error compiling shader %s:\n%s\n", path, log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLint loadShaderFromDisk(const char *vertexFile, const char *fragmentFile) {
    GLuint vertexShader = load_shader(vertexFile, GL_VERTEX_SHADER);
    GLuint fragmentShader = load_shader(fragmentFile, GL_FRAGMENT_SHADER);

    GLint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
