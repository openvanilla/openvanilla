#import "MyDelegate.h"

@implementation MyDelegate
- (IBAction)relaunch:(id)sender {
    fprintf (stderr, "relaunching\n");
    FILE *tmp=fopen("/tmp/~ovmrelaunch.sh", "w");
    fprintf(tmp, 
"osascript -e 'tell my application \"OVMonitor\" to quit'\n"
"osascript /tmp/~ovrelaunch.scpt\n"
"osascript -e 'tell my application \"OVMonitor\" to activate'\n"
);
    fclose(tmp);

    tmp=fopen("/tmp/~ovrelaunch.scpt", "w");
    fprintf(tmp, "tell my application \"OVMonitor\" to launch\n");
    fclose(tmp);
}
-(void)awakeFromNib {
    fprintf (stderr, "awakeFromNib!\n");
    
    monitorMessage=[[NSString alloc] initWithFormat:@"monitoring console.log for user %s\n", getenv("USER")];
    [console setString:monitorMessage];

    NSString *fn=[NSString stringWithFormat: @"/Library/Logs/Console/%s/console.log", getenv("USER")];
    [console insertText:fn];
    [console insertText:@" opened\n"];
    font=[[NSFont fontWithName:@"Monaco" size:12.0] retain];
    [console setFont:font];
    log=fopen([fn UTF8String], "r");
    if (!log) {
        fprintf (stderr, "cannot open log file %s, aborted\n", [fn UTF8String]);
        abort();
    }
    
    // skip until the end of the file
    fseek(log, 0, SEEK_END);
    
    [NSThread detachNewThreadSelector:@selector(monitor) toTarget:self withObject:nil];
    fprintf(stderr, "thread initialized!\n");
}
- (IBAction)clearButton:(id)sender
{
    [console setString:monitorMessage];
}
- (void)monitor {
    id pool=[NSAutoreleasePool new];
    fprintf(stderr, "thread entered!\n");
    
    [NSThread setThreadPriority:0];
    
    int i=0;
    char buf[8192];
    while (1) {
        i=0;
        while (1) {
            int c=fgetc(log);
            if (c==EOF) break;
            buf[i++]=c;
            if (i==8191 || c==0xa) break;
        }
        buf[i++]=0;
        
        if (i>1) {
            NSString *s=[[NSString alloc] initWithUTF8String: buf];
            [console insertText:s];
            [console setFont:font];
            [s release];
        }
        clearerr(log);
    }
    
    [pool release];
}
@end
