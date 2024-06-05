#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t cpid;
    char buf;

    // 파이프 생성
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 포크하여 자식 프로세스 생성
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {    // 자식 프로세스
        close(pipefd[1]);  // 쓰기 끝 닫기 (자식은 읽기만 할 것이므로)

        // 읽기 끝에서 데이터 읽기
        while (read(pipefd[0], &buf, 1) > 0) {
            write(STDOUT_FILENO, &buf, 1);  // 표준 출력으로 읽은 데이터 출력
        }

        close(pipefd[0]);  // 읽기 끝 닫기
        _exit(EXIT_SUCCESS);

    } else {            // 부모 프로세스
        close(pipefd[0]);  // 읽기 끝 닫기 (부모는 쓰기만 할 것이므로)

        // 쓰기 끝에 데이터 쓰기
        write(pipefd[1], "Hello from parent\n", 18);

        close(pipefd[1]);  // 쓰기 끝 닫기
        wait(NULL);        // 자식 프로세스 종료 대기
        exit(EXIT_SUCCESS);
    }
}

