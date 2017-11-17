#include <smon/smon.h>

#include <sys/vfs.h>
#include <sys/statvfs.h>

void test()
{
  smon::Manager& mon = smon::get_manager();
  mon.set_period(1);
  mon.start();
  mon.regist_proc("chrome");

  for (int i = 0; i<10; ++i) {

    printf("cpu : %s\n", mon.cpu().to_str().c_str());
    printf("mem : %s\n", mon.mem().to_str().c_str());
    printf("sys: %s\n", mon.sys().to_str().c_str());
    printf("disk: \n%s\n", mon.disk().to_str().c_str());
    printf("net: \n%s\n", mon.net().to_str().c_str());
    printf("proc: \n%s\n", mon.proc().to_str().c_str());

		std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

int main(int, char**)
{
	test();
	return 0;

	smon::CpuData cpu_data;
	smon::linux::CPU cpu;

	smon::DiskData disk_data;
	smon::linux::Disk disk;

	smon::MemData mem_data;
	smon::linux::Mem mem;

	smon::NetData net_data;
	smon::linux::Net net;

	smon::ProcData proc_data;
	smon::linux::Proc proc;
	while (true) {
		/*
		cpu.get(cpu_data);
    printf("cpu : %s\n", cpu_data.to_str().c_str());
		*/

		/*
		disk.get(disk_data);
    printf("disk : %s\n", disk_data.to_str().c_str());

		mem.get(mem_data);
    printf("mem : %s\n", mem_data.to_str().c_str());

		net.get(net_data);
    printf("net : %s\n", net_data.to_str().c_str());

    printf("sys : %s\n", smon::linux::SYS().get().to_str().c_str());
		*/

		proc.get(proc_data);
    printf("proc : %s\n", proc_data.to_str().c_str());

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

  //test();
  return 0;
}




