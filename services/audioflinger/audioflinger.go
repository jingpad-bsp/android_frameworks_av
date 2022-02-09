package audioflinger

import (
	"android/soong/android"
	"android/soong/cc"
	"fmt"
)

func init(){
	fmt.Println("audioflinger init start")
	android.RegisterModuleType("audioflinger_defaults",audioflinger_DefaultsFactory)
}

func audioflinger_DefaultsFactory() (android.Module) {
	module := cc.DefaultsFactory()
	fmt.Println("audioflinger_DefaultsFactory")
	android.AddLoadHook(module,audioflinger_defaults)
	return module
}

func audioflinger_defaults(ctx android.LoadHookContext){
	type props struct {
		Cflags []string
	}
	fmt.Println("audioflinger_defaults")
	p := &props{}
	p.Cflags = audioflinger_globalFlags(ctx)
	ctx.AppendProperties(p)
}

func audioflinger_globalFlags(ctx android.BaseContext) ([]string){
	var cflags []string

	buildVariantFlag := envDefault(ctx, "TARGET_BUILD_VARIANT", "null")
	fmt.Println("TARGET_BUILD_VARIANT:",buildVariantFlag)
	if buildVariantFlag == "userdebug"{
		cflags = append(cflags, "-DAUDIO_FW_PCM_DUMP")
	}
	return cflags
}

func envDefault(ctx android.BaseContext, key string, defaultValue string) string {
	ret := ctx.AConfig().Getenv(key)
	fmt.Println("envDefault key:",key)
	fmt.Println("envDefault ret:",ret)
	if ret == "" {
		return defaultValue
	}
	return ret
}

