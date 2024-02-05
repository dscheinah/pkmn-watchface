def options(ctx):
  ctx.add_option('--test', action='store_true', default=False, help='faster tick times for emulator testing')

def configure(ctx):
  if (ctx.options.test):
    ctx.env.append_value('DEFINES', 'TEST')
