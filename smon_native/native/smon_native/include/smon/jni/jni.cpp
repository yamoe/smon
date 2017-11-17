
#include "../smon.h"

#include "../../../../jni_header/io_yukul_smon_jni_JNI.h"


#include "env_wrapper.h"
#include "callbacker.h"
#include "data.h"

using namespace smon::jni;

jint JNICALL JNI_OnLoad
(JavaVM *jvm, void* /*reserved*/)
{
  return Callbacker::inst().on_load(jvm);
}

JNIEXPORT void JNICALL JNI_OnUnload
(JavaVM* /*vm*/, void* /*reserved*/)
{
}


JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_setPeriod
(JNIEnv* /*env*/, jobject /*obj*/, jint sec)
{
  if (sec < 1) return;
  smon::get_manager().set_period(sec);
}


JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_setCallbcak
(JNIEnv *env, jobject /*obj*/, jstring package_name, jstring method_name)
{
  EnvWrapper envw(env);
  
  std::string package = envw.get_string(package_name);
  std::string method = envw.get_string(method_name);

  try {
    Callbacker::inst().set_callbcak(package, method);
  }
  catch (const std::runtime_error&) {
  }
}


JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_start
(JNIEnv *, jobject)
{
  smon::get_manager().start();
}


JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_stop
(JNIEnv *, jobject)
{
  smon::get_manager().stop();
}


JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getCpu
(JNIEnv *env, jobject /*obj*/)
{
  try {
		auto data = smon::get_manager().cpu();
    return JniCpu::inst().clone(env, data);
  }
  catch (const std::runtime_error&) {
    return nullptr;
  }
}


JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getMem
(JNIEnv *env, jobject /*obj*/)
{
  try {
		auto data = smon::get_manager().mem();
    return JniMem::inst().clone(env, data);
  }
  catch (const std::runtime_error&) {
    return nullptr;
  }
}


JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getSys
(JNIEnv *env, jobject /*obj*/)
{
  try {
		auto data = smon::get_manager().sys();
    return JniSys::inst().clone(env, data);
  }
  catch (const std::runtime_error&) {
    return nullptr;
  }
}


JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getDisk
(JNIEnv *env, jobject /*obj*/)
{
  try {
		auto data = smon::get_manager().disk();
    return JniDisk::inst().clone(env, data);
  }
  catch (const std::runtime_error&) {
    return nullptr;
  }
}


JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getNet
(JNIEnv *env, jobject /*obj*/)
{
  try {
		auto data = smon::get_manager().net();
    return JniNet::inst().clone(env, data);
  }
  catch (const std::runtime_error&) {
    return nullptr;
  }
}


JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getProcList
(JNIEnv *env, jobject /*obj*/)
{
  try {
		auto data = smon::get_manager().proc_list();
    return JniProcList::inst().clone(env, data);
  }
  catch (const std::runtime_error&) {
    return nullptr;
  }
}

JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_registProc
(JNIEnv *env, jobject /*obj*/, jstring str)
{
  std::string pname = EnvWrapper(env).get_string(str);
  smon::get_manager().regist_proc(pname);
}

JNIEXPORT void JNICALL Java_io_yukul_smon_jni_JNI_unregistProc
(JNIEnv *env, jobject /*obj*/, jstring str)
{
  std::string pname = EnvWrapper(env).get_string(str);
  smon::get_manager().unregist_proc(pname);
}

JNIEXPORT jobject JNICALL Java_io_yukul_smon_jni_JNI_getProc
(JNIEnv *env, jobject /*obj*/)
{
  try {
		auto data = smon::get_manager().proc();
    return JniProc::inst().clone(env, data);
  }
  catch (const std::runtime_error&) {
    return nullptr;
  }
}

