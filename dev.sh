#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="./build"

# Helper Funtion to print fancy text on actions
printbox() {
  local msg="$1"
  local box_width=38                # fixed box width
  local msg_len=${#msg}

  # truncate if message is too long
  [ "$msg_len" -gt $((box_width - 4)) ] && msg="${msg:0:$((box_width - 4))}" && msg_len=${#msg}

  local padding=$(((box_width - 4 - msg_len)/2))
  local extra=$(((box_width - 4 - msg_len) % 2))  # for odd numbers

  local left_pad=$(printf '%*s' "$padding" '')
  local right_pad=$(printf '%*s' "$((padding + extra))" '')

  local border=$(printf '%*s' "$box_width" '' | tr ' ' '#')

  echo -e "\033[1;36m$border\033[0m"
  echo -e "\033[1;36m##${left_pad}${msg}${right_pad}##\033[0m"
  echo -e "\033[1;36m$border\033[0m"
}

# Select an executable from build dir
select_exec() {
  mapfile -t execs < <(find "$BUILD_DIR" -maxdepth 1 -type f -executable)
  [ ${#execs[@]} -eq 0 ] && return 1

  if [ ${#execs[@]} -eq 1 ]; then
    echo "${execs[0]#$BUILD_DIR/}"
  else
    printf "%s\n" "${execs[@]#$BUILD_DIR/}" | fzf --prompt="Select executable: "
  fi
}

# Build helper
build_project() {
  cmake -S . -B "$BUILD_DIR" -G Ninja
  ninja -C "$BUILD_DIR"
}

# Action handler
run_action() {
  local action="${1-}"   # default to empty if not set

  case "$action" in
    generate)
        printbox "Generate Buildfiles"
        time cmake -S . -B "$BUILD_DIR" -G Ninja
        ;;
    wipe)
        printbox "Wipe Buildfiles"
        echo "$(( $(rm -rvf "$BUILD_DIR" 2>/dev/null | wc -l) )) files deleted."
        ;;
    build)
        printbox "Build Project"
        time build_project
        ;;
    clean)
        printbox "Clean Buildfiles"
        ninja -C "$BUILD_DIR" clean
        ;;
    rebuild)
        printbox "Clean Buildfiles"
        echo "$(( $(rm -rvf "$BUILD_DIR" 2>/dev/null | wc -l) )) files deleted."
        printbox "Rebuild Project"
        time build_project
        ;;
    run)
        printbox "Build Project"
        time build_project
        printbox "Run Project"
        exec_file=$(select_exec) || { echo "No executable found."; return; }
        "$BUILD_DIR/$exec_file"
        ;;
    debug)
        time build_project
        printbox "Debug Project"
        exec_file=$(select_exec) || { echo "No executable found."; return; }
        gdb "$BUILD_DIR/$exec_file"
        ;;
    direct-run)
        printbox "Run Project"
        exec_file=$(select_exec) || { echo "No executable found."; return; }
        "$BUILD_DIR/$exec_file"
        ;;
    help|*|"")
        printbox "Help"
        echo "Usage: $0 {generate|wipe|build|clean|rebuild|run|debug|direct-run}"
      ;;
  esac
}

# If arguments are passed → run each in order, robustly
if [ $# -ge 1 ]; then
    for arg in "$@"; do
        run_action "$arg"
    done
    exit 0
fi

# Interactive FZF menu
actions=(
    "generate    → Generate Ninja build files"
    "wipe        → Wipe build directory"
    "build       → Build project"
    "clean       → Clean build"
    "rebuild     → Wipe + regenerate + build"
    "run         → Build + run executable"
    "debug       → Build + debug with gdb"
    "direct-run  → Run executable (no rebuild)"
    "exit        → Quit"
)

choice=$(printf "%s\n" "${actions[@]}" | fzf --prompt="Select action: " --height=20% --reverse | awk '{print $1}')
[ -n "${choice-}" ] && [ "$choice" != "exit" ] && run_action "$choice"

