#pragma once

#include <map>
#include <string>
#include <algorithm>

#include <sys/vfs.h>

#include "file.h"

namespace smon {
  namespace linux {

    class Disk : public smon::Disk {
		private:
			typedef std::map<std::string, std::string> MountInfo; //device, path

			class DiskIO {
			public:
				uint64_t sectors_read_ = 0;
				uint64_t sectors_write_ = 0;

				bool is_overflow(DiskIO& now)
				{
					if (now.sectors_read_ < sectors_read_ ||
							now.sectors_write_ < sectors_write_)
					{
						return true;
					}
					return false;
				}
			};
			typedef std::map<std::string, DiskIO> PreDiskIO;
			PreDiskIO pre_disk_io_;


    public:
      Disk() {}
      virtual ~Disk() override {}

			virtual void get(DiskData& data) override
			{
				data.clear();

			  set_total_and_use(data.disk_);
				set_io(data.disk_);
			}


		private:

			void set_total_and_use(std::map<std::string, DiskUsage>& disk)
			{
				auto mtab = get_mount_info("/etc/mtab");
				auto mounts = get_mount_info("/proc/mounts");

				MountInfo info;
				MountInfo::iterator info_it(info.begin());
				std::set_intersection(
					mtab.begin(), mtab.end(),
					mounts.begin(), mounts.end(), 
					std::inserter(info, info_it)
				);

				for (auto it : info) {
					DiskUsage& usage = disk[it.first];
					set_disk_usage(it.second, usage);
				}
			}

			void set_disk_usage(const std::string& path, DiskUsage& usage)
			{
				struct statfs stat;
				if (statfs(path.c_str(), &stat) == -1) {
					return;
				}

				// to blocks
				/*
				uint64_t avail = stat.f_bavail * (stat.f_bsize / 1024);
				total = stat.f_blocks * (stat.f_bsize / 1024);
				use = total - avail;
				*/

				// to bytes
				uint64_t avail = stat.f_bavail * stat.f_frsize;
				usage.total_ = stat.f_blocks * stat.f_frsize;
				usage.use_ = usage.total_ - avail;
			}


			MountInfo get_mount_info(const char* filename)
			{
				MountInfo mi;
				File file;
				if (!file.open(filename)) {
					Log::err("%s %s", filename, file.err().c_str());
					return mi;
				}

				char line[512] = {0,};
				char device[64] = {0,};
				char path[256] = {0,};

				while (file.get(line)) {
					if (strncmp(line, "/dev/", 5)) {
						continue;
					}
					sscanf(line, "%s %s", device, path);
					mi.emplace(device, path);
				}

				return mi;
			}

			void set_io(std::map<std::string, DiskUsage>& disk)
			{
				const char* filename = "/proc/diskstats";

				File file;
				if (!file.open(filename)) {
					Log::err("%s %s", filename, file.err().c_str());
					return;
				}

				// https://www.kernel.org/doc/Documentation/ABI/testing/procfs-diskstats
				uint64_t dum = 0;
				char device_name[10] = {0,};
				uint64_t sectors_read = 0;
				uint64_t sectors_written = 0;

				char line[512] = {0,};
				while (file.get(line)) {
					sscanf(line, "%lu %lu %s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
						&dum, // major_number
						&dum, // minor_number
						device_name,
						&dum, // reads_completed_successfully
						&dum, // reads_merged
						&sectors_read,
						&dum, // time_spent_reading_ms
						&dum, // writes_completed
						&dum, // writes_merged
						&sectors_written,
						&dum, // time_spent_writing_ms
						&dum, // ios_currently_in_progress
						&dum, // time_spent_doing_ios_ms
						&dum  // weighted_time_spent_doing_ios_ms
					);

					auto it = disk.find(std::string("/dev/") + device_name);
					if (it == disk.end()) {
						continue;
					}

					auto& pre_disk_ios = pre_disk_io_;

					auto jt = pre_disk_ios.find(it->first);
					if (jt == pre_disk_ios.end()) {

						DiskIO io;
						io.sectors_read_ = sectors_read;
						io.sectors_write_ = sectors_written;

						pre_disk_ios[it->first] = io;
					} else {

						DiskIO now_io;
						now_io.sectors_read_ = sectors_read;
						now_io.sectors_write_ = sectors_written;

						auto& pre_io = jt->second;
						if (pre_io.is_overflow(now_io)) {
							pre_disk_ios.erase(jt);
							continue;
						}

						auto& usage = it->second;

						// 1 sector is 512 bytes (or check /sys/block/sda/queue/hw_sector_size)
						usage.io_read_ = (now_io.sectors_read_ - pre_io.sectors_read_) * 512;
						usage.io_write_ = (now_io.sectors_write_ - pre_io.sectors_write_) * 512;
						usage.io_total_ = usage.io_read_ + usage.io_write_;

						pre_io = now_io;
					}
				}
				
			}

			std::vector<std::string> partitions()
			{
				std::vector<std::string> names;

				const char* filename = "/proc/partitions";

				File file;
				if (!file.open(filename)) {
					Log::err("%s %s", filename, file.err().c_str());
					return names;
				}

				char line[256] = {0,};
				uint32_t major = 0;
				uint32_t minor = 0;
				uint64_t blocks = 0;
				char name[64] = {0,};

				while (file.get(line)) {
					sscanf(line, "%u %u %lu %s", &major, &minor, &blocks, name);
					
					if (strlen(name) <= 3) {
						continue;
					}
					
					if (strncmp(name, "sd", 2)) {
						continue;
					}

					names.push_back(name);
				}
				return names;

			}
    };

  }
}

