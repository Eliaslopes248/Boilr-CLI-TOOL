#!/bin/bash
set -e
# GET SCRIPT PATH
SCRIPT_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 || exit ; pwd -P )"
echo $SCRIPT_DIR
echo "===================================="
echo "CREATING RELEASE"
echo "===================================="

# CHECKING IF GITHUB CLI IS INSTALLED
if ! command -v gh > /dev/null 2>&1; then
    echo "[ERROR] Github CLI not installed"
    echo "[ERROR] Install Github CLI (brew install gh)"
    exit 1
fi

# GET RELEASE NUMBER via ARGS
RELEASE_TITLE="DEFAULT"
RELEASE_VERSION=""
RELEASE_NOTES="No notes"

while [ $# -gt 0 ]; do
    case "$1" in
        -t)
            RELEASE_TITLE="$2"
            shift 2
            ;;
        -v)
            RELEASE_VERSION="$2"
            shift 2
            ;;
        -n)
            RELEASE_NOTES="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            shift
            ;;
    esac
done

# PRINT OUT ARGS
echo "VERSION: $RELEASE_VERSION"
echo "NOTES:   $RELEASE_NOTES"

echo "Collecting release builds"

# FETCHING ALL BUILD .ZIP FILES
FILES=($SCRIPT_DIR/build-*.zip)


# MAKING RELEASE
gh release create $RELEASE_VERSION "${FILES[@]}"   \
    --title "$RELEASE_TITLE"                         \
    --notes "$RELEASE_NOTES"
