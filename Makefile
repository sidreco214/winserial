#사용할 컴파일러
CC = g++
#컴파일러에 전달할 argument
CFLAGS = -O2 -Wall -std=c++17

#소스+헤더파일 경로
SOURCEDIR = src
#빌드시 나올 오브젝트 파일과 실행 파일 저장위치
BUILDDIR = build

#실행파일 이름
EXECUTABLE = serial.exe
#실행파일 실행시 전달할 argument, 아무것도 없는 경우 RUNFLAG =
RUNFLAG = -p COM3

#main 함수가 정의된 파일 이름
MAINSRC = main.cpp
MAINFOBJ = $(BUILDDIR)/$(patsubst %.cpp,%.o,$(MAINSRC))

#patsubst = pattern substitude
SOURCES = $(wildcard $(SOURCEDIR)/*.cpp)
OBJECTS = $(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

#그냥 make 하면 make all로 인식됨
all: dir $(BUILDDIR)/$(EXECUTABLE)

#build 폴더가 없을 때만 폴더를 생성하도록 함
dir: $(BUILDDIR)
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

#링크 과정
$(BUILDDIR)/$(EXECUTABLE): $(OBJECTS) $(MAINFOBJ)
	$(CC) $^ -o $@

#소스파일 컴파일, 소스파일 안에 include로 헤더를 포함하기 때문에
#소스파일만 컴파일 하면 됨, 다만 해더파일이 변경되었을 때도 새로 컴파일 하도록 종속성에 추가함
$(OBJECTS): $(SOURCES) $(patsubst %.cpp,%.h,$(SOURCES))
	$(CC) $< -c -o $@ $(CFLAGS)

#메인 함수있는 파일 컴파일
$(MAINFOBJ): $(MAINSRC)
	$(CC) $(MAINSRC) -c -o $@ $(CFLAGS)

#build 폴더내 오브젝트 파일과 실행파일 비우기
clean:
	rm -f $(BUILDDIR)/*o $(BUILDDIR)/$(EXECUTABLE)

#프로그램 실행
run:
	$(BUILDDIR)/$(EXECUTABLE) $(RUNFLAG)