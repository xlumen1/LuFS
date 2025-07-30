# LuFS

This is a repository containing a standalone library for the [LumenOS](https://github.com/xlumen1/LumenOS) Filesystem (LuFS).
This will be updated along side LumenOS, although updates may not be instant.

---

If you just want to install the latest version of the library, run this command:
```bash
git clone https://github.com/xlumen1/LuFS && cd LuFS && make all && sudo cp out/liblufs.so /usr/local/lib/ && sudo ldconfig && sudo cp src/lufs.h /usr/local/include && cd .. && rm -rf LuFS
```

Then, just include lufs.h in a c program.

## Examples

Load an image
```c
#include <stdio.h>
#include <lufs.h>

int main() {
	LuFS* fs = lufs_open("disk.img");
	if ( !fs ) {
		fprintf(stderr, "Failed to load filesystem.\n");
		return 1;
	}
	lufs_close(fs);
	return 0;
}
```

