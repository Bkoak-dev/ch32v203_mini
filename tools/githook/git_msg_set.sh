#!/bin/bash

#for linux
#tools/githook absolute directory
githook_dir=$(cd $(dirname $0);pwd)

#.git/ absolute directory
project_dir=$(cd $(dirname $0);cd ../..;pwd)
git_dir=$project_dir/.git/hooks

#add commit template configuration
git config --global commit.template $githook_dir/commit_message
git config --global core.editor vim

#commit-msg hook is used for check commit message format when execute git commit
#if message format doesn't match the rules, this commit will be blocked by commit-msg hook
cp $githook_dir/commit-msg $git_dir/commit-msg

#check git hook
if [ -e "$git_dir/commit-msg" ]
then
	echo "git configuration OK!"
else
	echo "ERROR! Configuration failed!"
fi
