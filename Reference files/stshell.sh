#!/bin/bash

function execute_command {
  # Split the command into an array
  local -a cmd=($1)
  
  # Check if the command contains a pipe
  local i=0
  local has_pipe=0
  for arg in "${cmd[@]}"; do
    if [[ $arg == "|" ]]; then
      has_pipe=1
      break
    fi
    i=$((i + 1))
  done
  
  if [[ $has_pipe -eq 0 ]]; then
    # No pipe, execute the command with output redirection if specified
    if [[ ${#cmd[@]} -ge 3 && (${cmd[-2]} == ">" || ${cmd[-2]} == ">>") ]]; then
      ${cmd[@]::${#cmd[@]}-2} ${cmd[-2]} ${cmd[-1]}
    else
      "${cmd[@]}"
    fi
  else
    # Pipe, execute the commands with piping
    local -a cmd1=("${cmd[@]::i}")
    local -a cmd2=("${cmd[@]:i+1}")
    local tmp=$(mktemp)
    
    "${cmd1[@]}" > $tmp
    
    for arg in "${cmd2[@]}"; do
      if [[ $arg == "|" ]]; then
        # Find the next command in the pipeline
        local j=$((i + 1))
        local -a cmd3=("${cmd2[@]:j+1}")
        local tmp2=$(mktemp)
        
        "${cmd2[@]::$j}" < $tmp > $tmp2
        
        # Swap the temp files and commands for the next iteration
        i=$j
        cmd2=("${cmd3[@]}")
        tmp=$tmp2
      fi
    done
    
    # Execute the last command with output redirection if specified
    if [[ ${#cmd2[@]} -ge 2 && (${cmd2[-2]} == ">" || ${cmd2[-2]} == ">>") ]]; then
      "${cmd2[@]::${#cmd2[@]}-2}" < $tmp ${cmd2[-2]} ${cmd2[-1]}
    else
      "${cmd2[@]}" < $tmp
    fi
    
    rm $tmp
  fi
}

function handle_sigint {
  echo "Caught SIGINT, stopping command"
}

function stshell {
  while true; do
    read -p "$ " input
    
    if [[ $input == "exit" ]]; then
      break
    fi
    
    # Execute the command in a subshell to allow Ctrl+c to work
    (trap handle_sigint SIGINT; execute_command "$input")
  done
}

stshell

