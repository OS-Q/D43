Import("env")

env.Prepend(LINKFLAGS=[
  "--specs=nosys.specs"
])

# env.Append(LINKFLAGS=["--specs=nosys.specs", "--specs=nano.specs"])
