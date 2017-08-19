#include <stdio.h>
#include <jni.h>
#include <stdlib.h>
#include "com_example_lame_MainActivity.h"
#include "lame.h"
#include <android/log.h>
#include <malloc.h>
#define LOG_TAG "System.out"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__);
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);
char* Jstring2CStr(JNIEnv* env, jstring jstr){
	char *rtn = NULL;
	jclass clsstring = (*env)->FindClass(env, "java/lang/String");
	jstring strencode = (*env)->NewStringUTF(env, "GB2312");
	jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray barr= (jbyteArray)(*env)->CallObjectMethod(env, jstr, mid,
			strencode);
	jsize alen = (*env)->GetArrayLength(env, barr);
	jbyte* ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
	if(alen > 0){
		rtn = (char*) malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	(*env)->ReleaseByteArrayElements(env, barr, ba, 0);
	return rtn;
}
void setProcess(JNIEnv *env, jobject thiz, int process) {
	char *classname = "com/example/lame/MainActivity";
	jclass clazz;
	clazz = (*env)->FindClass(env, classname);
	if(clazz == 0) {
		LOGI("CAN'T FIND CLAZZ");
	}else {
		LOGI("FIND CLAZZ");
	}
	jmethodID java_method = (*env)->GetMethodID(env, clazz, "setProgressbar",
				"(I)V");
	if(java_method == 0) {
			LOGI("CAN'T FIND METHOD");
		}else {
			LOGI("FIND METHOD");
		}
	(*env)->CallVoidMethod(env, thiz, java_method, process);
}
JNIEXPORT jstring JNICALL Java_com_example_lame_MainActivity_getLameVersion
  (JNIEnv *env, jobject obj){
	return (*env)->NewStringUTF(env, get_lame_version());
}
JNIEXPORT void JNICALL Java_com_example_lame_MainActivity_convertAudio
  (JNIEnv *env, jobject obj, jstring input, jstring output){
	char *inputname = Jstring2CStr(env, input);
	char *outputname = Jstring2CStr(env, output);
	LOGI("INPUT NAME = %s", inputname);
	LOGI("OUTPUT NAME = %s", outputname);
	LOGI("init");
	FILE * wav = fopen(inputname, "rb");
	FILE * mp3 = fopen(outputname, "wb");
	int read;
	int write;
	short int wav_buffer[8192*2];
	unsigned char mp3_buffer[8192];
	lame_t lame = lame_init();
	lame_set_in_samplerate(lame, 44100);
	lame_set_num_channels(lame, 2);
	lame_set_VBR(lame, vbr_default);
	lame_init_params(lame);
	LOGI("lame init finish");
	int total = 0;
	do {
		read = fread(wav_buffer, sizeof(short int)*2, 8192, wav);
		total += read * 4;
		setProcess(env, obj, total);
		if(read == 0) {
			write = lame_encode_flush(lame, mp3_buffer, 8192);
			fwrite(mp3_buffer, sizeof(char), write, mp3);
		}else {
			write = lame_encode_buffer_interleaved(lame, wav_buffer, read, mp3_buffer, 8192);
			fwrite(mp3_buffer, sizeof(char), write, mp3);
		}
	}while(read != 0);
	LOGI("encode finish");
	lame_close(lame);
	fclose(mp3);
	fclose(wav);
	LOGI("convert completed");
}
