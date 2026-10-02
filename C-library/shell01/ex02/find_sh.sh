find . -type f -name "*.sh"  -printf "%f\n" | cut -d "." -f1 
