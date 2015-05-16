var byebye_Win32Helper = require( '../' )
var test = require( 'tape' )
var child_process = require( 'child_process' )

test('Detection', function (t) {
    t.timeoutAfter(10000)
    t.plan(1)

    t.strictEqual( byebye_Win32Helper.addon_loaded, true, 'addon binary found and loaded' )
    
    t.end()
})

test('CloseMainWindowsByProcess', function (t) {
    t.timeoutAfter(30000)
    t.plan(3)

    var expectExit = false
    var lastChance
    var proc = child_process.spawn( 'notepad.exe' )
    t.notStrictEqual( proc, undefined, "Launched 'notepad.exe'" )
    proc.on('exit', function onProcNone_Exit( code, signal ) {
        if( lastChance ) {
            clearTimeout( lastChance )
        }
        if( expectExit ) {
            t.strictEqual( signal, null, "'notepad.exe' didn't caught any signal" )
        } else {
            t.fail( "unexpected 'notepad.exe' exit" )
        }
        t.end()
    })
    lastChance = setTimeout(function safetyKill() {
        proc.kill('SIGKILL')
        t.fail( "emergency kill" )
        t.end()
    }, 7000 )
    setTimeout(function() {
        t.doesNotThrow(function() {
            expectExit = true
            byebye_Win32Helper.CloseMainWindowsByProcess( proc )
        }, "calling CloseMainWindowsByProcess() didn't threw an exception")
    }, 2000 )
})
