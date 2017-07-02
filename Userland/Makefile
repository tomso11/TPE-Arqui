include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

all: sampleCodeModule sampleDataModule superUserModule shellModule dumbModule

sampleCodeModule:
	cd SampleCodeModule; make

shellModule:
	cd ShellModule; make

superUserModule:
	cd SuperUserModule; make
dumbModule:
	cd DumbModule; make
sampleDataModule:
	printf "This is sample data." >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
	cd ShellModule; make clean
	cd DumbModule; make clean
	cd SuperUserModule; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule  all clean
