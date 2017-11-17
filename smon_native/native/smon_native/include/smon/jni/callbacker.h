#pragma once

namespace smon {
  namespace jni {

    // smon manager의 콜백을 jni로 전달해주는 singleton 클래스
    class Callbacker : public Manager::Callback {
    private:
      static const jint jni_version_ = JNI_VERSION_1_8;

      JavaVM* jvm_ = nullptr;

      class Info {
      public:
        jclass cls_ = nullptr;
        jmethodID method_ = nullptr;
      };


      // lock for cb_
      std::mutex mutex_;

      // key = pakcege +"/"+ method
      std::unordered_map<std::string, Info> cb_;

    private:
      Callbacker()
      {
        smon::get_manager().set_callback(this);
      }

      ~Callbacker() {}

    public:

      virtual void on_callback() override
      {
        callback_to_jvm();
      }

      static Callbacker& inst()
      {
        static Callbacker* inst = new Callbacker;
        return *inst;
      }

      jint on_load(JavaVM* jvm)
      {
        jvm_ = jvm;

        JNIEnv* env;
        if (jvm->GetEnv((void **)&env, jni_version_) != JNI_OK) {

          //TODO: error
          return -1;
        }
        return jni_version_;
      }

      void set_callbcak(const std::string& package, const std::string& method)
      {
        std::lock_guard<std::mutex> lock(mutex_);

        // already exist
        std::string key = package + "/" + method;
        if (cb_.find(key) != cb_.end()) {
          return;
        }

        JNIEnv* env = get_env();

        EnvWrapper envw(env);
        if (!envw) {
          //TODO: error
          return;
        }

        Info info;
        info.cls_ = envw.find_global_class(package.c_str());
        info.method_ = envw.get_static_method_id(info.cls_, method.c_str(), "()V");

        cb_.emplace(key, info);
      }

      void callback_to_jvm()
      {
        std::lock_guard<std::mutex> lock(mutex_);

        if (cb_.empty()) return;

        JNIEnv* env = get_env();
        EnvWrapper envw(env);
        if (!envw) {
          //TODO: error
          return;
        }

        try {
          for (auto& it : cb_) {
            auto& info = it.second;
            envw->CallStaticVoidMethod(info.cls_, info.method_);
            envw.except("");
          }
        }
        catch (const std::runtime_error&) {
          // nothing to do 
        }
      }

      JNIEnv* get_env()
      {
        JNIEnv* env = nullptr;
        void** env_void = reinterpret_cast<void**>(&env);
        jint ret = jvm_->GetEnv(env_void, jni_version_);

        switch (ret) {
        case JNI_OK:
          return env;
        case JNI_EDETACHED:
          if (jvm_->AttachCurrentThread(env_void, NULL) < 0) {
            return nullptr;
          }
          else {
            return env;
          }
        case JNI_EVERSION:
          //TODO: error
          return nullptr;
        default:
          //TODO: error
          return nullptr;
        }
      }



    };

  }
}


