# Producers-Consumers TLA+ example

## How to run

### Using command line

Install `tla-bin`(https://github.com/pmer/tla-bin)

Run model-checking 
```bash
tlc -workers 4 -cleanup ProducersConsumers.tla
```
if you want to check all state space. It automatically uses ProducersConsumers.cfg config file.

### Using VSCode

- Install TLA+ VSCode Extension
- Ctrl+Shift+P for command pallete 
- Find "TLA+: Check model with TLC"

It automatically uses ProducersConsumers.cfg config file.

### Using TLA+ Toolbox

If you want GUI with all possible model configurations
- Install `TLA+ Toolbox`(https://github.com/tlaplus/tlaplus/releases/tag/v1.7.1#latest-tla-files)
- Open ProducersConsumers.tla spec
- Create new model
- Configure model options according to config file
- Run model 