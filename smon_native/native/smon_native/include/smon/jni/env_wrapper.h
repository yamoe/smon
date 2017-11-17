#pragma once

namespace smon {
  namespace jni {

    class EnvWrapper {
    protected:
      JNIEnv* env_ = nullptr;

    public:
      EnvWrapper(JNIEnv* env)
        : env_(env)
      {
      }

      JNIEnv* operator->()
      {
        return env_;
      }

      operator bool()
      {
        return (env_ != nullptr);
      }

      // jstring -> std::string
      std::string get_string(jstring jstr)
      {
        const char* ch = env_->GetStringUTFChars(jstr, nullptr);
        std::string str(ch);
        env_->ReleaseStringUTFChars(jstr, ch);
        return str;
      }

      jstring new_string(const std::string& str)
      {
        return env_->NewStringUTF(str.c_str());
      }

      jclass find_class(const char* name)
      {
        jclass cls = env_->FindClass(name);
        if (!cls) {
          except("FindClass(%s)", name);
        }
        return cls;
      }

      jclass find_global_class(const char* name)
      {
        jclass cls = find_class(name);
        jobject obj = env_->NewGlobalRef(cls);
        env_->DeleteLocalRef(cls);
        return reinterpret_cast<jclass>(obj);
      }

      jmethodID get_method_id(jclass cls, const char* name, const char* sig)
      {
        jmethodID mid = env_->GetMethodID(cls, name, sig);
        if (!mid) {
          except("GetMethodID(%s, %s)", name, sig);
        }
        return mid;
      }

      jmethodID get_static_method_id(jclass cls, const char* name, const char* sig)
      {
        jmethodID mid = env_->GetStaticMethodID(cls, name, sig);
        if (!mid) {
          except("GetStaticMethodID(%s, %s)", name, sig);
        }
        return mid;
      }

      jobject new_object(jclass cls, jmethodID mid, ...) {
        va_list args;
        jobject result;
        va_start(args, mid);
        result = env_->NewObjectV(cls, mid, args);
        va_end(args);

        if (!result) {
          except("NewObject");
        }
        return result;

      }

      template <std::size_t size = 1024>
      void except(const char* format, ...)
      {
        if (env_ && env_->ExceptionOccurred()) {
          va_list args;
          va_start(args, format);
          Log::err(format, args);
          va_end(args);

          env_->ExceptionDescribe();
          env_->ExceptionClear();

          throw std::runtime_error("");
        }
      }

    };

  }
}