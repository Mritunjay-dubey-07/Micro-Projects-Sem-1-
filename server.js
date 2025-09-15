const express = require('express');
const { execFile } = require('child_process');
const path = require('path');
const app = express();
const port = 3000;

// Serve static files (HTML, CSS, client-side JS)
app.use(express.static(__dirname));
// Middleware to parse JSON and URL-encoded data
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// Handle signup form submission
app.post('/signup', (req, res) => {
    const {
        'account-number': accountNumber,
        'ifsc-code': ifscCode,
        fullname,
        email,
        username,
        password
    } = req.body;

    const executablePath = path.join(__dirname, 'database'); // Assumes compiled C++ is named 'database'

    // Execute the compiled C++ program
    execFile(executablePath, [accountNumber, ifscCode, fullname, email, username, password], (error, stdout, stderr) => {
        if (error) {
            console.error(`execFile error: ${error}`);
            return res.status(500).json({ success: false, message: 'Server error occurred.' });
        }
        
        const result = stdout.trim();
        if (result.startsWith('SUCCESS')) {
            res.json({ success: true, message: 'Account created successfully!' });
        } else {
            // Send back the specific error message from the C++ program
            res.status(400).json({ success: false, message: result.replace('ERROR: ', '') });
        }
    });
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
    console.log('Serving files from:', __dirname);
});
