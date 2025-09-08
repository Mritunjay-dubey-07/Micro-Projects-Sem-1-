
// Wait for DOM to be fully loaded
document.addEventListener('DOMContentLoaded', function() {
    
    // Get all interactive elements
    const loginForm = document.getElementById('login-form');
    const usernameInput = document.getElementById('username');
    const passwordInput = document.getElementById('password');
    const primaryBtns = document.querySelectorAll('.primary-btn');
    const secondaryBtns = document.querySelectorAll('.secondary-btn');
    const textBtns = document.querySelectorAll('.text-btn');
    
    // Add ripple effect to buttons
    function createRipple(event) {
        const button = event.currentTarget;
        const circle = document.createElement('span');
        const diameter = Math.max(button.clientWidth, button.clientHeight);
        const radius = diameter / 2;
        
        circle.style.width = circle.style.height = `${diameter}px`;
        circle.style.left = `${event.clientX - button.offsetLeft - radius}px`;
        circle.style.top = `${event.clientY - button.offsetTop - radius}px`;
        circle.classList.add('ripple');
        
        // Add ripple styles
        circle.style.position = 'absolute';
        circle.style.borderRadius = '50%';
        circle.style.background = 'rgba(255, 255, 255, 0.4)';
        circle.style.transform = 'scale(0)';
        circle.style.animation = 'ripple 0.6s linear';
        circle.style.pointerEvents = 'none';
        
        // Remove existing ripples
        const ripple = button.getElementsByClassName('ripple')[0];
        if (ripple) {
            ripple.remove();
        }
        
        button.appendChild(circle);
    }
    
    // Add CSS animation for ripple effect
    const style = document.createElement('style');
    style.textContent = `
        @keyframes ripple {
            to {
                transform: scale(4);
                opacity: 0;
            }
        }
        
        .input-focus-glow {
            box-shadow: 0 0 20px rgba(50, 130, 184, 0.3);
        }
        
        .btn-click-animation {
            animation: btnClick 0.1s ease;
        }
        
        @keyframes btnClick {
            0% { transform: scale(1); }
            50% { transform: scale(0.95); }
            100% { transform: scale(1); }
        }
        
        .shake {
            animation: shake 0.5s ease-in-out;
        }
        
        @keyframes shake {
            0%, 100% { transform: translateX(0); }
            25% { transform: translateX(-5px); }
            75% { transform: translateX(5px); }
        }
    `;
    document.head.appendChild(style);
    
    // Add ripple effect to all buttons
    [...primaryBtns, ...secondaryBtns].forEach(button => {
        button.addEventListener('click', createRipple);
        
        // Add click animation
        button.addEventListener('mousedown', function() {
            this.classList.add('btn-click-animation');
        });
        
        button.addEventListener('animationend', function() {
            this.classList.remove('btn-click-animation');
        });
    });
    
    // Enhanced input focus effects for login form
    if (usernameInput && passwordInput) {
        [usernameInput, passwordInput].forEach(input => {
            input.addEventListener('focus', function() {
                this.parentElement.style.transform = 'scale(1.02)';
                this.parentElement.classList.add('input-focus-glow');
            });
            
            input.addEventListener('blur', function() {
                this.parentElement.style.transform = 'scale(1)';
                this.parentElement.classList.remove('input-focus-glow');
            });
            
            // Add typing animation
            input.addEventListener('input', function() {
                this.style.transform = 'scale(1.01)';
                setTimeout(() => {
                    this.style.transform = 'scale(1)';
                }, 100);
            });
        });
    }
    
    // Form submission handling
    if (loginForm) {
        loginForm.addEventListener('submit', function(e) {
        e.preventDefault();
        
        const username = usernameInput.value.trim();
        const password = passwordInput.value.trim();
        
        if (!username || !password) {
            // Shake animation for empty fields
            if (!username) {
                usernameInput.parentElement.classList.add('shake');
                usernameInput.focus();
            }
            if (!password && username) {
                passwordInput.parentElement.classList.add('shake');
                passwordInput.focus();
            }
            
            // Remove shake animation after completion
            setTimeout(() => {
                usernameInput.parentElement.classList.remove('shake');
                passwordInput.parentElement.classList.remove('shake');
            }, 500);
            
            return;
        }
        
        // Simulate login process
        const loginBtn = document.querySelector('.login-btn');
        const originalText = loginBtn.querySelector('.btn-text').textContent;
        
        loginBtn.querySelector('.btn-text').textContent = 'Signing In...';
        loginBtn.style.background = 'linear-gradient(135deg, #28a745, #20c997)';
        loginBtn.disabled = true;
        
        // Simulate API call delay
        setTimeout(() => {
            loginBtn.querySelector('.btn-text').textContent = '✓ Success!';
            loginBtn.style.background = 'linear-gradient(135deg, #28a745, #20c997)';
            
            setTimeout(() => {
                alert(`Welcome back, ${username}! Login successful.`);
                loginBtn.querySelector('.btn-text').textContent = originalText;
                loginBtn.style.background = 'linear-gradient(135deg, var(--highlight-color), #5ba7db)';
                loginBtn.disabled = false;
                
                // Reset form
                loginForm.reset();
            }, 1000);
        }, 1500);
        });
    }
    
    // Handle other buttons - only if they exist
    const signupBtn = document.querySelector('.signup-btn');
    if (signupBtn) {
        signupBtn.addEventListener('click', function() {
            window.location.href = 'signup.html';
        });
    }
    
    const forgotBtn = document.querySelector('.forgot-btn');
    if (forgotBtn) {
        forgotBtn.addEventListener('click', function() {
            alert('Password reset link will be sent to your email.');
        });
    }

    // Handle signup form if on signup page
    const signupForm = document.getElementById('signup-form');
    if (signupForm) {
        const accountNumberInput = document.getElementById('account-number');
        const ifscCodeInput = document.getElementById('ifsc-code');
        const fullnameInput = document.getElementById('fullname');
        const emailInput = document.getElementById('email');
        const signupUsernameInput = document.getElementById('username');
        const signupPasswordInput = document.getElementById('password');
        const confirmPasswordInput = document.getElementById('confirm-password');
        
        // Add focus effects to all signup inputs
        [accountNumberInput, ifscCodeInput, fullnameInput, emailInput, signupUsernameInput, signupPasswordInput, confirmPasswordInput].forEach(input => {
            if (input) {
                input.addEventListener('focus', function() {
                    this.parentElement.style.transform = 'scale(1.02)';
                    this.parentElement.classList.add('input-focus-glow');
                });
                
                input.addEventListener('blur', function() {
                    this.parentElement.style.transform = 'scale(1)';
                    this.parentElement.classList.remove('input-focus-glow');
                });
                
                input.addEventListener('input', function() {
                    this.style.transform = 'scale(1.01)';
                    setTimeout(() => {
                        this.style.transform = 'scale(1)';
                    }, 100);
                });
            }
        });

        // Add ripple effects to signup form buttons
        const signupBtns = signupForm.querySelectorAll('.primary-btn, .secondary-btn');
        signupBtns.forEach(button => {
            button.addEventListener('click', createRipple);
            
            button.addEventListener('mousedown', function() {
                this.classList.add('btn-click-animation');
            });
            
            button.addEventListener('animationend', function() {
                this.classList.remove('btn-click-animation');
            });
        });

        signupForm.addEventListener('submit', function(e) {
            e.preventDefault();
            
            const accountNumber = accountNumberInput.value.trim();
            const ifscCode = ifscCodeInput.value.trim().toUpperCase();
            const fullname = fullnameInput.value.trim();
            const email = emailInput.value.trim();
            const username = signupUsernameInput.value.trim();
            const password = signupPasswordInput.value.trim();
            const confirmPassword = confirmPasswordInput.value.trim();
            
            // Validation
            if (!accountNumber || !ifscCode || !fullname || !email || !username || !password || !confirmPassword) {
                alert('Please fill in all fields.');
                // Add shake effect to empty fields
                if (!accountNumber) accountNumberInput.parentElement.classList.add('shake');
                if (!ifscCode) ifscCodeInput.parentElement.classList.add('shake');
                if (!fullname) fullnameInput.parentElement.classList.add('shake');
                if (!email) emailInput.parentElement.classList.add('shake');
                if (!username) signupUsernameInput.parentElement.classList.add('shake');
                if (!password) signupPasswordInput.parentElement.classList.add('shake');
                if (!confirmPassword) confirmPasswordInput.parentElement.classList.add('shake');
                
                setTimeout(() => {
                    [accountNumberInput, ifscCodeInput, fullnameInput, emailInput, signupUsernameInput, signupPasswordInput, confirmPasswordInput].forEach(input => {
                        if (input) input.parentElement.classList.remove('shake');
                    });
                }, 500);
                return;
            }
            
            // Validate account number format (10-12 digits)
            if (!/^[0-9]{10,12}$/.test(accountNumber)) {
                alert('Account number must be 10-12 digits long.');
                accountNumberInput.parentElement.classList.add('shake');
                setTimeout(() => {
                    accountNumberInput.parentElement.classList.remove('shake');
                }, 500);
                return;
            }
            
            // Validate IFSC code format (4 letters + 7 digits)
            if (!/^[A-Z]{4}[0-9]{7}$/.test(ifscCode)) {
                alert('IFSC code must be in format: 4 letters followed by 7 digits (e.g., BODD0000001).');
                ifscCodeInput.parentElement.classList.add('shake');
                setTimeout(() => {
                    ifscCodeInput.parentElement.classList.remove('shake');
                }, 500);
                return;
            }
            
            if (password !== confirmPassword) {
                alert('Passwords do not match.');
                confirmPasswordInput.parentElement.classList.add('shake');
                setTimeout(() => {
                    confirmPasswordInput.parentElement.classList.remove('shake');
                }, 500);
                return;
            }
            
            // Simulate signup process
            const signupSubmitBtn = document.querySelector('.signup-submit-btn');
            const originalText = signupSubmitBtn.querySelector('.btn-text').textContent;
            
            signupSubmitBtn.querySelector('.btn-text').textContent = 'Creating Account...';
            signupSubmitBtn.style.background = 'linear-gradient(135deg, #28a745, #20c997)';
            signupSubmitBtn.disabled = true;
            
            setTimeout(() => {
                signupSubmitBtn.querySelector('.btn-text').textContent = '✓ Account Created!';
                
                setTimeout(() => {
                    alert(`Welcome to Bank of Diddy Oil, ${fullname}! Your account has been created successfully.`);
                    window.location.href = 'index.html';
                }, 1000);
            }, 1500);
        });
    }

    // Handle back to login button
    const backToLoginBtn = document.querySelector('.back-to-login-btn');
    if (backToLoginBtn) {
        backToLoginBtn.addEventListener('click', function() {
            window.location.href = 'index.html';
        });
    }
    
    // Add smooth scrolling and parallax effect
    window.addEventListener('scroll', function() {
        const scrolled = window.pageYOffset;
        const parallax = document.querySelector('.background-blur');
        const speed = scrolled * 0.5;
        
        parallax.style.transform = `translateY(${speed}px)`;
    });
    
    // Add hover sound effect simulation (visual feedback)
    function addHoverEffect(element) {
        element.addEventListener('mouseenter', function() {
            this.style.filter = 'brightness(1.1)';
        });
        
        element.addEventListener('mouseleave', function() {
            this.style.filter = 'brightness(1)';
        });
    }
    
    // Apply hover effects to interactive elements
    [...primaryBtns, ...secondaryBtns, ...textBtns].forEach(addHoverEffect);
    
    // Add floating animation to logo
    const logo = document.querySelector('.logo-text');
    setInterval(() => {
        logo.style.transform = 'scale(1.02)';
        setTimeout(() => {
            logo.style.transform = 'scale(1)';
        }, 200);
    }, 3000);
    
    // Dynamic background color shift
    let hue = 210;
    setInterval(() => {
        hue = (hue + 0.5) % 360;
        document.documentElement.style.setProperty('--accent-color', `hsl(${hue}, 60%, 25%)`);
    }, 100);
    
    console.log('🏦 Bank of Diddy Oil - Advanced UI Loaded Successfully!');
});

