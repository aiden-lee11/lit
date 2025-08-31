## commands todo 

lit init --> creates a .lit/ directory with like HEAD, objects/ refs/
lit hash-object --> takes file and writes the blob to objects gives back hash 



lit add <filename> --> adds the file to be tracked as an object --> create a hash of the content and store like below in the objects directory

```
If you again examine your objects directory, you can see that it now contains a file for that new content. This is how Git stores the content initially — as a single file per piece of content, named with the SHA-1 checksum of the content and its header. The subdirectory is named with the first 2 characters of the SHA-1, and the filename is the remaining 38 characters.
```

lit rm <filename> --> removes the file from being tracked in objects

