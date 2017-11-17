#pragma once

#include "env_wrapper.h"

// smon data와 java data로의 변환

namespace smon {
  namespace jni {

    class JniCpu {
    public:

      jclass cpu_cls_ = nullptr;
      jmethodID cpu_ctor_ = nullptr;

      jmethodID cpu_set_utc_ = nullptr;
      jmethodID cpu_set_total_ = nullptr;
      jmethodID cpu_set_cores_ = nullptr;

      jclass arr_cls_ = nullptr;
      jmethodID arr_ctor_ = nullptr;
      jmethodID arr_add_ = nullptr;

      jclass int_cls_ = nullptr;
      jmethodID int_ctor_ = nullptr;

    private:
      JniCpu()
      {
        if (cpu_cls_) return;

        JNIEnv* env = Callbacker::inst().get_env();
        EnvWrapper envw(env);

        cpu_cls_ = envw.find_global_class("io/yukul/smon/jni/dto/CpuData");
        cpu_ctor_ = envw.get_method_id(cpu_cls_, "<init>", "()V");

        cpu_set_utc_ = envw.get_method_id(cpu_cls_, "setUtc", "(J)V");
        cpu_set_total_ = envw.get_method_id(cpu_cls_, "setTotal", "(I)V");
        cpu_set_cores_ = envw.get_method_id(cpu_cls_, "setCores", "(Ljava/util/ArrayList;)V");

        arr_cls_ = envw.find_global_class("java/util/ArrayList");
        arr_ctor_ = envw.get_method_id(arr_cls_, "<init>", "()V");
        arr_add_ = envw.get_method_id(arr_cls_, "add", "(Ljava/lang/Object;)Z");

        int_cls_ = envw.find_global_class("java/lang/Integer");
        int_ctor_ = envw.get_method_id(int_cls_, "<init>", "(I)V");
      }

    public:
      static JniCpu& inst()
      {
        static JniCpu inst_;
        return inst_;
      }

      jobject clone(JNIEnv* env, CpuData& data)
      {
        EnvWrapper envw(env);

        jobject arr_obj = envw.new_object(arr_cls_, arr_ctor_);

        for (auto it : data.cores_) {
          jobject int_obj = envw.new_object(int_cls_, int_ctor_, it);

          envw->CallBooleanMethod(arr_obj, arr_add_, int_obj);
          
          envw->DeleteLocalRef(int_obj);
        }

        jobject obj = envw.new_object(cpu_cls_, cpu_ctor_);
        envw->CallVoidMethod(obj, cpu_set_utc_, data.utc_);
        envw->CallVoidMethod(obj, cpu_set_total_, data.total_);
        envw->CallVoidMethod(obj, cpu_set_cores_, arr_obj);

        envw->DeleteLocalRef(arr_obj);
        return obj;
      }

    };


    class JniMem {
    public:

      jclass mem_cls_ = nullptr;
      jmethodID mem_ctor_ = nullptr;

      jmethodID mem_set_utc_ = nullptr;
      jmethodID mem_set_total_ = nullptr;
      jmethodID mem_set_use_ = nullptr;

    private:
      JniMem()
      {
        if (mem_cls_) return;

        JNIEnv* env = Callbacker::inst().get_env();
        EnvWrapper envw(env);

        mem_cls_ = envw.find_global_class("io/yukul/smon/jni/dto/MemData");
        mem_ctor_ = envw.get_method_id(mem_cls_, "<init>", "()V");

        mem_set_utc_ = envw.get_method_id(mem_cls_, "setUtc", "(J)V");
        mem_set_total_ = envw.get_method_id(mem_cls_, "setTotal", "(J)V");
        mem_set_use_ = envw.get_method_id(mem_cls_, "setUse", "(J)V");
      }

    public:
      static JniMem& inst()
      {
        static JniMem inst_;
        return inst_;
      }

      jobject clone(JNIEnv* env, MemData& data)
      {
        EnvWrapper envw(env);

        jobject obj = envw.new_object(mem_cls_, mem_ctor_);
        envw->CallVoidMethod(obj, mem_set_utc_, data.utc_);
        envw->CallVoidMethod(obj, mem_set_total_, data.total_bytes_);
        envw->CallVoidMethod(obj, mem_set_use_, data.use_bytes_);
        return obj;
      }

    };

    class JniSys {
    public:

      jclass sys_cls_ = nullptr;
      jmethodID sys_ctor_ = nullptr;

      jmethodID sys_set_utc_ = nullptr;
      jmethodID sys_set_hostname_ = nullptr;
      jmethodID sys_set_osname_ = nullptr;
      jmethodID sys_set_processor_ = nullptr;
      jmethodID sys_set_baseboard_ = nullptr;
      jmethodID sys_set_ram_ = nullptr;
      jmethodID sys_set_disk_ = nullptr;
      jmethodID sys_set_ip_ = nullptr;

    private:
      JniSys()
      {
        if (sys_cls_) return;

        JNIEnv* env = Callbacker::inst().get_env();
        EnvWrapper envw(env);

        sys_cls_ = envw.find_global_class("io/yukul/smon/jni/dto/SysData");
        sys_ctor_ = envw.get_method_id(sys_cls_, "<init>", "()V");

        sys_set_utc_ = envw.get_method_id(sys_cls_, "setUtc", "(J)V");
        sys_set_hostname_ = envw.get_method_id(sys_cls_, "setHostName", "(Ljava/lang/String;)V");
        sys_set_osname_ = envw.get_method_id(sys_cls_, "setOsName", "(Ljava/lang/String;)V");
        sys_set_processor_ = envw.get_method_id(sys_cls_, "setProcessor", "(Ljava/lang/String;)V");
        sys_set_baseboard_ = envw.get_method_id(sys_cls_, "setBaseboard", "(Ljava/lang/String;)V");
        sys_set_ram_ = envw.get_method_id(sys_cls_, "setRam", "(Ljava/lang/String;)V");
        sys_set_disk_ = envw.get_method_id(sys_cls_, "setDisk", "(Ljava/lang/String;)V");
        sys_set_ip_ = envw.get_method_id(sys_cls_, "setIp", "(Ljava/lang/String;)V");

      }

    public:
      static JniSys& inst()
      {
        static JniSys inst_;
        return inst_;
      }

      jobject clone(JNIEnv* env, SysData& data)
      {
        EnvWrapper envw(env);

        jobject obj = envw.new_object(sys_cls_, sys_ctor_);

        envw->CallVoidMethod(obj, sys_set_utc_, data.utc_);
        set_str(envw, obj, sys_set_hostname_, data.host_name_);
        set_str(envw, obj, sys_set_osname_, data.os_name_);
        set_str(envw, obj, sys_set_processor_, data.processor_);
        set_str(envw, obj, sys_set_baseboard_, data.baseboard_);
        set_str(envw, obj, sys_set_ram_, data.ram_);
        set_str(envw, obj, sys_set_disk_, data.disk_);
        set_str(envw, obj, sys_set_ip_, data.ip_);

        return obj;
      }

    private:
      void set_str(EnvWrapper& envw, jobject obj, jmethodID mid, const std::string& str)
      {
        jstring jstr = envw.new_string(str);
        envw->CallVoidMethod(obj, mid, jstr);
        envw->DeleteLocalRef(jstr);
      }
    };


    class JniDisk {
    public:
      jclass disk_cls_ = nullptr;
      jmethodID disk_ctor_ = nullptr;

      jmethodID disk_set_utc_ = nullptr;
      jmethodID disk_set_disk_ = nullptr;

      jclass tree_cls_ = nullptr;
      jmethodID tree_ctor_ = nullptr;
      jmethodID tree_put_ = nullptr;

      jclass usage_cls_ = nullptr;
      jmethodID usage_ctor_ = nullptr;
      jmethodID usage_set_total_ = nullptr;
      jmethodID usage_set_use_ = nullptr;
      jmethodID usage_set_io_total_ = nullptr;
      jmethodID usage_set_io_read_ = nullptr;
      jmethodID usage_set_io_write_ = nullptr;

    private:
      JniDisk()
      {
        if (disk_cls_) return;

        JNIEnv* env = Callbacker::inst().get_env();
        EnvWrapper envw(env);

        disk_cls_ = envw.find_global_class("io/yukul/smon/jni/dto/DiskData");
        disk_ctor_ = envw.get_method_id(disk_cls_, "<init>", "()V");

        disk_set_utc_ = envw.get_method_id(disk_cls_, "setUtc", "(J)V");
        disk_set_disk_ = envw.get_method_id(disk_cls_, "setDisk", "(Ljava/util/TreeMap;)V");

        tree_cls_ = envw.find_global_class("java/util/TreeMap");
        tree_ctor_ = envw.get_method_id(tree_cls_, "<init>", "()V");
        tree_put_ = envw.get_method_id(tree_cls_, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

        usage_cls_ = envw.find_global_class("io/yukul/smon/jni/dto/DiskUsage");
        usage_ctor_ = envw.get_method_id(usage_cls_, "<init>", "()V");
        usage_set_total_ = envw.get_method_id(usage_cls_, "setTotal", "(J)V");
        usage_set_use_ = envw.get_method_id(usage_cls_, "setUse", "(J)V");
        usage_set_io_total_ = envw.get_method_id(usage_cls_, "setIoTotal", "(J)V");
        usage_set_io_read_ = envw.get_method_id(usage_cls_, "setIoRead", "(J)V");
        usage_set_io_write_ = envw.get_method_id(usage_cls_, "setIoWrite", "(J)V");
      }

    public:
      static JniDisk& inst()
      {
        static JniDisk inst_;
        return inst_;
      }

      jobject clone(JNIEnv* env, DiskData& data)
      {
        EnvWrapper envw(env);

        jobject tree_obj = envw.new_object(tree_cls_, tree_ctor_);

        for (auto& it : data.disk_) {

          jstring key = envw.new_string(it.first);

          jobject usage_obj = envw.new_object(usage_cls_, usage_ctor_);
          envw->CallVoidMethod(usage_obj, usage_set_total_, it.second.total_);
          envw->CallVoidMethod(usage_obj, usage_set_use_, it.second.use_);
          envw->CallVoidMethod(usage_obj, usage_set_io_total_, it.second.io_total_);
          envw->CallVoidMethod(usage_obj, usage_set_io_read_, it.second.io_read_);
          envw->CallVoidMethod(usage_obj, usage_set_io_write_, it.second.io_write_);
          envw->CallObjectMethod(tree_obj, tree_put_, key, usage_obj);

          envw->DeleteLocalRef(key);
          envw->DeleteLocalRef(usage_obj);
        }

        jobject obj = envw.new_object(disk_cls_, disk_ctor_);
        envw->CallVoidMethod(obj, disk_set_utc_, data.utc_);
        envw->CallVoidMethod(obj, disk_set_disk_, tree_obj);

        envw->DeleteLocalRef(tree_obj);
        return obj;
      }
    };

    class JniNet {
    public:
      jclass net_cls_ = nullptr;
      jmethodID net_ctor_ = nullptr;

      jmethodID net_set_utc_ = nullptr;
      jmethodID net_set_net_ = nullptr;

      jclass tree_cls_ = nullptr;
      jmethodID tree_ctor_ = nullptr;
      jmethodID tree_put_ = nullptr;

      jclass usage_cls_ = nullptr;
      jmethodID usage_ctor_ = nullptr;
      jmethodID usage_set_total_ = nullptr;
      jmethodID usage_set_sent_ = nullptr;
      jmethodID usage_set_recved_ = nullptr;

    private:
      JniNet()
      {
        if (net_cls_) return;

        JNIEnv* env = Callbacker::inst().get_env();
        EnvWrapper envw(env);

        net_cls_ = envw.find_global_class("io/yukul/smon/jni/dto/NetData");
        net_ctor_ = envw.get_method_id(net_cls_, "<init>", "()V");

        net_set_utc_ = envw.get_method_id(net_cls_, "setUtc", "(J)V");
        net_set_net_ = envw.get_method_id(net_cls_, "setNet", "(Ljava/util/TreeMap;)V");

        tree_cls_ = envw.find_global_class("java/util/TreeMap");
        tree_ctor_ = envw.get_method_id(tree_cls_, "<init>", "()V");
        tree_put_ = envw.get_method_id(tree_cls_, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

        usage_cls_ = envw.find_global_class("io/yukul/smon/jni/dto/NetUsage");
        usage_ctor_ = envw.get_method_id(usage_cls_, "<init>", "()V");
        usage_set_total_ = envw.get_method_id(usage_cls_, "setTotal", "(J)V");
        usage_set_sent_ = envw.get_method_id(usage_cls_, "setSent", "(J)V");
        usage_set_recved_ = envw.get_method_id(usage_cls_, "setRecved", "(J)V");
      }

    public:
      static JniNet& inst()
      {
        static JniNet inst_;
        return inst_;
      }

      jobject clone(JNIEnv* env, NetData& data)
      {
        EnvWrapper envw(env);

        jobject tree_obj = envw.new_object(tree_cls_, tree_ctor_);

        for (auto& it : data.net_) {

          jstring key = envw.new_string(it.first);

          jobject usage_obj = envw.new_object(usage_cls_, usage_ctor_);
          envw->CallVoidMethod(usage_obj, usage_set_total_, it.second.total_);
          envw->CallVoidMethod(usage_obj, usage_set_sent_, it.second.sent_);
          envw->CallVoidMethod(usage_obj, usage_set_recved_, it.second.recved_);
          envw->CallObjectMethod(tree_obj, tree_put_, key, usage_obj);

          envw->DeleteLocalRef(key);
          envw->DeleteLocalRef(usage_obj);
        }

        jobject obj = envw.new_object(net_cls_, net_ctor_);
        envw->CallVoidMethod(obj, net_set_utc_, data.utc_);
        envw->CallVoidMethod(obj, net_set_net_, tree_obj);

        envw->DeleteLocalRef(tree_obj);
        return obj;
      }
    };

    class JniProcList {
    public:
      jclass tree_cls_ = nullptr;
      jmethodID tree_ctor_ = nullptr;
      jmethodID tree_add_ = nullptr;

    private:
      JniProcList()
      {
        if (tree_cls_) return;

        JNIEnv* env = Callbacker::inst().get_env();
        EnvWrapper envw(env);

        tree_cls_ = envw.find_global_class("java/util/TreeSet");
        tree_ctor_ = envw.get_method_id(tree_cls_, "<init>", "()V");
        tree_add_ = envw.get_method_id(tree_cls_, "add", "(Ljava/lang/Object;)Z");

      }

    public:
      static JniProcList& inst()
      {
        static JniProcList inst_;
        return inst_;
      }

      jobject clone(JNIEnv* env, std::set<std::string>& data)
      {
        EnvWrapper envw(env);

        jobject obj = envw.new_object(tree_cls_, tree_ctor_);
        for (auto& it : data) {
          jstring str = envw.new_string(it);

          envw->CallBooleanMethod(obj, tree_add_, str);

          envw->DeleteLocalRef(str);
        }
        return obj;
      }
    };

    class JniProc {
    public:
      jclass proc_cls_ = nullptr;
      jmethodID proc_ctor_ = nullptr;

      jmethodID proc_set_utc_ = nullptr;
      jmethodID proc_set_proc_ = nullptr;

      jclass tree_cls_ = nullptr;
      jmethodID tree_ctor_ = nullptr;
      jmethodID tree_put_ = nullptr;

      jclass arr_cls_ = nullptr;
      jmethodID arr_ctor_ = nullptr;
      jmethodID arr_add_ = nullptr;

      jclass usage_cls_ = nullptr;
      jmethodID usage_ctor_ = nullptr;
      jmethodID usage_set_pid_ = nullptr;
      jmethodID usage_set_cpu_ = nullptr;
      jmethodID usage_set_pmem_ = nullptr;
      jmethodID usage_set_vmem_ = nullptr;
      jmethodID usage_set_io_total_ = nullptr;
      jmethodID usage_set_io_read_ = nullptr;
      jmethodID usage_set_io_write_ = nullptr;
      jmethodID usage_set_th_cnt_ = nullptr;
      jmethodID usage_set_tcp_cnt_ = nullptr;
      jmethodID usage_set_tcp_listen_cnt_ = nullptr;
      jmethodID usage_set_tcp_estab_cnt_ = nullptr;
      jmethodID usage_set_udp_cnt_ = nullptr;

    private:
      JniProc()
      {
        if (proc_cls_) return;

        JNIEnv* env = Callbacker::inst().get_env();
        EnvWrapper envw(env);

        proc_cls_ = envw.find_global_class("io/yukul/smon/jni/dto/ProcData");
        proc_ctor_ = envw.get_method_id(proc_cls_, "<init>", "()V");

        proc_set_utc_ = envw.get_method_id(proc_cls_, "setUtc", "(J)V");
        proc_set_proc_ = envw.get_method_id(proc_cls_, "setProc", "(Ljava/util/TreeMap;)V");

        tree_cls_ = envw.find_global_class("java/util/TreeMap");
        tree_ctor_ = envw.get_method_id(tree_cls_, "<init>", "()V");
        tree_put_ = envw.get_method_id(tree_cls_, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

        arr_cls_ = envw.find_global_class("java/util/ArrayList");
        arr_ctor_ = envw.get_method_id(arr_cls_, "<init>", "()V");
        arr_add_ = envw.get_method_id(arr_cls_, "add", "(Ljava/lang/Object;)Z");

        usage_cls_ = envw.find_global_class("io/yukul/smon/jni/dto/ProcUsage");
        usage_ctor_ = envw.get_method_id(usage_cls_, "<init>", "()V");
        usage_set_pid_ = envw.get_method_id(usage_cls_, "setPid", "(I)V");
        usage_set_cpu_ = envw.get_method_id(usage_cls_, "setCpu", "(I)V");
        usage_set_pmem_ = envw.get_method_id(usage_cls_, "setPmem", "(J)V");
        usage_set_vmem_ = envw.get_method_id(usage_cls_, "setVmem", "(J)V");
        usage_set_io_total_ = envw.get_method_id(usage_cls_, "setIoTotal", "(J)V");
        usage_set_io_read_ = envw.get_method_id(usage_cls_, "setIoRead", "(J)V");
        usage_set_io_write_ = envw.get_method_id(usage_cls_, "setIoWrite", "(J)V");
        usage_set_th_cnt_ = envw.get_method_id(usage_cls_, "setThCnt", "(I)V");
        usage_set_tcp_cnt_ = envw.get_method_id(usage_cls_, "setTcpCnt", "(I)V");
        usage_set_tcp_listen_cnt_ = envw.get_method_id(usage_cls_, "setTcpListenCnt", "(I)V");
        usage_set_tcp_estab_cnt_ = envw.get_method_id(usage_cls_, "setTcpEstabCnt", "(I)V");
        usage_set_udp_cnt_ = envw.get_method_id(usage_cls_, "setUdpCnt", "(I)V");
      }

    public:
      static JniProc& inst()
      {
        static JniProc inst_;
        return inst_;
      }

      jobject clone(JNIEnv* env, ProcData& data)
      {
        EnvWrapper envw(env);

        jobject tree_obj = envw.new_object(tree_cls_, tree_ctor_);

        for (auto& it : data.proc_) {
          jstring pname = envw.new_string(it.first);
          jobject arr_obj = envw.new_object(arr_cls_, arr_ctor_);

          for (auto& usage : it.second) {
            jobject usage_obj = envw.new_object(usage_cls_, usage_ctor_);
            envw->CallVoidMethod(usage_obj, usage_set_pid_, usage.pid_);
            envw->CallVoidMethod(usage_obj, usage_set_cpu_, usage.cpu_);
            envw->CallVoidMethod(usage_obj, usage_set_pmem_, usage.pmem_);
            envw->CallVoidMethod(usage_obj, usage_set_vmem_, usage.vmem_);
            envw->CallVoidMethod(usage_obj, usage_set_io_total_, usage.io_total_);
            envw->CallVoidMethod(usage_obj, usage_set_io_read_, usage.io_read_);
            envw->CallVoidMethod(usage_obj, usage_set_io_write_, usage.io_write_);
            envw->CallVoidMethod(usage_obj, usage_set_th_cnt_, usage.th_cnt_);
            envw->CallVoidMethod(usage_obj, usage_set_tcp_cnt_, usage.tcp_cnt_);
            envw->CallVoidMethod(usage_obj, usage_set_tcp_listen_cnt_, usage.tcp_listen_cnt_);
            envw->CallVoidMethod(usage_obj, usage_set_tcp_estab_cnt_, usage.tcp_estab_cnt_);
            envw->CallVoidMethod(usage_obj, usage_set_udp_cnt_, usage.udp_cnt_);

            envw->CallBooleanMethod(arr_obj, arr_add_, usage_obj);
          }

          envw->CallObjectMethod(tree_obj, tree_put_, pname, arr_obj);

          envw->DeleteLocalRef(pname);
          envw->DeleteLocalRef(arr_obj);
        }

        jobject obj = envw.new_object(proc_cls_, proc_ctor_);
        envw->CallVoidMethod(obj, proc_set_utc_, data.utc_);
        envw->CallVoidMethod(obj, proc_set_proc_, tree_obj);

        envw->DeleteLocalRef(tree_obj);
        return obj;
      }
    };

  }
}
