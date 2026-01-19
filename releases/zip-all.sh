for dir in build-*; do
  if [ -d "$dir" ]; then
    zip -r "${dir}.zip" "$dir"
  fi
done
