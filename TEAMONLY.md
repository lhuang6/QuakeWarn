# How to Edit
First, open up VSCode on the cs50.dev website to open up the CS50 computer. When you first enter CS50, it defaults to opening up a random folder with
a bunch of numbers. This folder is basically a different GitHub repository that automatically updates, so normally, all of your work can be saved to be
submitted to Harvard. To get to the actual computer, run:

```bash
cd workspaces
```

Now that you are in the folder outside, you are free to do whatever you like with CS50's virtual machine. Run "git clone" followed by the git link that you
can find on the green code dropdown button at the top. Example:

```bash
git clone https://github.com/lhuang6/QuakeWarn.git
```

Now, you should see a brand new folder that is called "QuakeWarn" that is an exact clone of this GitHub website.

Once you are done run "git add" followed by the files you edited: Run "git commit -m " and follow it with two quotation marks and put a description of what you
changed. Finally, run "git push" to put it on the website. Example:
```bash
git add main.c
git commit -m "Editing Main File"
git push
```
