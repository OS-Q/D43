Import("env")
import shutil
import os

env.Append(LINKFLAGS=["--specs=nano.specs"])
# env.Append(LINKFLAGS=["--specs=nosys.specs", "--specs=nano.specs"])

def after_build(source, target, env):
	isExists=os.path.exists('build')
	if not isExists:
		os.mkdir('build')
	shutil.copy(firmware_source, 'build/release.bin')

env.AddPostAction("buildprog", after_build)

firmware_source = os.path.join(env.subst("$BUILD_DIR"), "firmware.bin")
