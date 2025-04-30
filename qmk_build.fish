#!/usr/bin/env fish

# Load environment variables from .env file
if test -f .env
    for line in (cat .env | grep -v '^#')
        set -l kv (string split = $line)
        if test (count $kv) -eq 2
            set -gx $kv[1] $kv[2]
        end
    end
end

# Execute the original script with all arguments passed through
./util/docker_build.sh $argv
