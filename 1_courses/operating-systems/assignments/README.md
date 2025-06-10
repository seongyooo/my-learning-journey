2025_01/OS/README
# RWlock 

## 목적
- The Readers-Writers Problem을 multi-thread 환경에서 Synchronization tool인 semaphore를 통해 mutual exclution과 fairness를 보장하도록 구현하였다. 

## 구현방법 (step-by-step)
1. 입출력 및 main 루프 구현
    * `argc`, `argv`를 통해 파일 읽기 
    * `fgets()`으로 한줄 읽기
    * `sscanf()`로 read/write과 processing time 파싱
    * `strcmp()`로 read와 write 구분
    * `usleep()`으로 100ms 지연

2. 스레드 생성 및 reader & writer 구현
    * `pthread_create()`, `pthread_join()`
    * `reader()`, `writer()` print 형식 구현
    * `thread_args` 구조체 

3. time stamp 구현
    * `[second.micro second(4자리)]` 구현
    * `gettimeofday()` 사용
    * `time_stamp_cal()` -> 시간 계산하는 함수 

4. 기본적인 lock 구현
    * `rwlock_t` 구조체
    * `rwlock_init()`
    * `readlock()`, `writelock()` acquire, release 구현
    * `lock()`들 적용

5. fairness를 위한 lock 구현
    * `fariness` 세마포어 변수 추가
    * `fariness` lock을 통해 공정성 구현

## 사용법
1. 프로그램 컴파일
    * `make`

2. 프로그램 실행 예시
    * `./rwlock sequence.txt`

3. 프로그램 삭제
    * `make clean`

### Referenced
- 블로그: https://www.joinc.co.kr/w/man/2/gettimeofday -> 시스템 시간값 계산
