#define OV_DEBUG
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <OpenVanilla/OpenVanilla.h>
#include <OpenVanilla/OVLibrary.h>
#include <OpenVanilla/OVUtility.h>

#define PHP_PATH "/usr/bin/php"
#define OUT_PATH "/tmp/ovphp-out.tmp"
#define IN_PATH "/tmp/ovphp-in.tmp"

const char *convert(const char *string, const char *command, char *buf){
	struct stat st;
	char buffer[1024];
	stat(PHP_PATH, &st);
	if(!st.st_size)
		return "";

	FILE *fp=fopen(OUT_PATH, "w");
	if (!fp) return "";
	fprintf(fp, "<?php \n error_reporting(0);\n");
	fprintf(fp, "echo %s(\"%s\");", command, string);
	fprintf(fp, "\n?>");
	fclose(fp);

	sprintf(buffer, "%s %s > %s", PHP_PATH, OUT_PATH, IN_PATH);
	system(buffer);

	strcpy(buffer, "");

	fp=fopen(IN_PATH, "r");
	if (!fp) return 0;
	while (!feof(fp))
	{
		fgets(buffer, sizeof(buffer), fp);
		strcat(buf, buffer);
	}
	fclose(fp);
	unlink(OUT_PATH);
	return (const char*)buf;
}


class OVOFmd5 : public OVOutputFilter {
public:
    OVOFmd5() {
    }
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
        const char *modulePath) {
        return 1;
    }
    
    virtual const char *identifier() { return "OVOFmd5"; }
    virtual const char *localizedName(const char *locale)
    {return "PHP md5";}
    virtual const char *process (const char *src, OVService *srv) {
        if (buf) free(buf);
        buf=(char*)calloc(1, strlen(src)+1);
	strcpy(buf, convert(src, "md5", buf));
        return buf;
    }
    
protected:
    char *buf;
};

class OVOFurlencode : public OVOutputFilter {
public:
    OVOFurlencode() {
    }
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
						   const char *modulePath) {
        return 1;
    }
    
    virtual const char *identifier() { return "OVOFurlencode"; }
    virtual const char *localizedName(const char *locale)
    {return "PHP urlencode";}
    virtual const char *process (const char *src, OVService *srv) {
        if (buf) free(buf);
        buf=(char*)calloc(1, strlen(src)+1);
	strcpy(buf, convert(src, "urlencode", buf));
	return buf;
    }
    
protected:
    char *buf;
};

class OVOFhtmlentities : public OVOutputFilter {
public:
    OVOFhtmlentities() {
    }
    
    virtual int initialize(OVDictionary *moduleCfg, OVService *srv, 
						   const char *modulePath) {
        return 1;
    }
    
    virtual const char *identifier() { return "OVOFhtmlentities"; }
    virtual const char *localizedName(const char *locale)
    {return "PHP htmlentities";}
    virtual const char *process (const char *src, OVService *srv) {
        if (buf) free(buf);
        buf=(char*)calloc(1, strlen(src)+1);
	strcpy(buf, convert(src, "htmlentities", buf));
	return buf;
    }
    
protected:
    char *buf;
};

// OV_SINGLE_MODULE_WRAPPER(OVOFBasicFilter);

extern "C" unsigned int OVGetLibraryVersion() {
    return OV_VERSION;
}

extern "C" int OVInitializeLibrary(OVService* s, const char* p) { 
    return 1; 
}

extern "C" OVModule *OVGetModuleFromLibrary(int idx) {
	switch (idx) {
		case 0: return new OVOFmd5;
		case 1: return new OVOFurlencode;
		case 2: return new OVOFhtmlentities;
	}
	return NULL;
}

